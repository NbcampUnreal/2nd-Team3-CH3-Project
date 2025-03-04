

#include "Character/MyCharacter.h"
#include "Character/MyPlayerController.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/Firearm.h"
#include "Weapon/MeleeWeapon.h"
#include "Weapon/Parts.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ChildActorComponent.h"
#include "UI/HexPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Core/HexboundGameInstance.h"
#include "Managers/UIManager.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//스프링 암
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	//카메라
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	//속도 관련
	NormalSpeed = 600.0f;
	SprintSpeedMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	CrouchSpeedMultiplier = 0.5f;
	CrouchSpeed = NormalSpeed * CrouchSpeedMultiplier;

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	bUseControllerRotationYaw = true;

	if (CameraComp)
	{
		DefaultFOV = CameraComp->FieldOfView;
	}

	// Weapon ChildActorComponent 설정
	WeaponSlot = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponSlot"));
	WeaponSlot->SetupAttachment(GetMesh(), TEXT("RightHand")); // Skeletal Mesh의 Slot 이름

	// 임시로 추가해둠 (수정 필요)
	Magazine = CreateDefaultSubobject<UChildActorComponent>(TEXT("Magazine"));
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UHexPlayerHUD>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();

			// 초기 체력 반영
			HUDWidget->UpdateHealth(Health, MaxHealth);

			// 초기 탄약 반영 (필요하다면)
			// HUDWidget->UpdateAmmo(CurrentAmmo, MaxAmmo);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUDWidgetClass is NULL!"));
	}
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentYaw = GetActorRotation().Yaw;

	if (CameraComp)
	{
		float TargetFOV = bIsZooming ? ZoomedFOV : DefaultFOV;
		float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
		CameraComp->SetFieldOfView(NewFOV);
	}
	if (Controller)
	{
		FRotator ControlRotation = Controller->GetControlRotation();
		FRotator TargetRotation(0.f, ControlRotation.Yaw, 0.f);
		FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
		SetActorRotation(SmoothRotation);

		// Yaw Offset 업데이트
		float NewYawOffset = CurrentYaw - PreviousYaw;
		NewYawOffset = FMath::Clamp(NewYawOffset, -90.f, 90.f);
		YawOffset = NewYawOffset;

		//if (FMath::Abs(YawOffset) > 5.f)
		//{
		//	PreviousYaw = CurrentYaw;
		//	UE_LOG(LogTemp, Warning, TEXT("Updated PreviousYaw: %f"), PreviousYaw);
		//}

		//if (FMath::Abs(YawOffset) < 5.0f)
		//{
		//	YawOffset = FMath::FInterpTo(YawOffset, 0.f, DeltaTime, 5.0f);
		//}

		//if (FMath::Abs(YawOffset) > 45.f)
		//{
		//	if (!bIsTurning)
		//	{
		//		PlayAnimMontage(Turn90Anim);
		//		bIsTurning = true;
		//	}
		//}
		//else if (bIsTurning && FMath::Abs(YawOffset) < 5.f)
		//{
		//	StopAnimMontage(Turn90Anim);
		//	bIsTurning = false;
		//}

		/*UE_LOG(LogTemp, Warning, TEXT("YawOffset: %f, PreviousYaw: %f"), YawOffset, PreviousYaw);*/
	}
	if (bInterpToThirdPerson)
	{
		if (SpringArmComp)
		{
			CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);

			float NewTargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, 300.f, DeltaTime, 5.f);
			SpringArmComp->TargetArmLength = NewTargetArmLength;
		}

		FVector NewLocation = FMath::VInterpTo(CameraComp->GetComponentLocation(), SpringArmComp->GetComponentLocation(), DeltaTime, 10.f);
		FRotator NewRotation = FMath::RInterpTo(CameraComp->GetComponentRotation(), SpringArmComp->GetComponentRotation(), DeltaTime, 10.f);

		CameraComp->SetWorldLocation(NewLocation);
		CameraComp->SetWorldRotation(NewRotation);

		if (FVector::Dist(NewLocation, SpringArmComp->GetComponentLocation()) < 1.f &&
			FMath::Abs(NewRotation.Yaw - SpringArmComp->GetComponentRotation().Yaw) < 1.f &&
			FMath::Abs(SpringArmComp->TargetArmLength - 300.f) < 1.f)
		{
			bInterpToThirdPerson = false;

			CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
			CameraComp->bUsePawnControlRotation = false;
		}
	}

}

#pragma region Character Movement
// ======================  캐릭터 이동관련  ===========================
void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller || !CameraComp) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (MoveInput.IsNearlyZero()) return;

	// 카메라의 회전 정보 가져오기
	const FRotator CameraRotation = CameraComp->GetComponentRotation();
	const FRotator YawRotation(0.f, CameraRotation.Yaw, 0.f); // Pitch와 Roll 제거, Yaw만 유지

	// 캐릭터가 항상 카메라 방향을 보게 설정
	SetActorRotation(YawRotation);

	// 카메라 기준의 전/후 방향
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 카메라 기준의 좌/우 방향
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 이동 입력 적용
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(ForwardDirection, MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(RightDirection, MoveInput.Y);
	}
}
void AMyCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Jump();
	}
}
void AMyCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}
void AMyCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}
void AMyCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void AMyCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}
void AMyCharacter::StartCrouch(const FInputActionValue& value)
{
	if (!bIsCrouching)
	{
		Crouch();
		bIsCrouching = true;
		if (GetCharacterMovement()) {
			GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
		}
	}
}
void AMyCharacter::StopCrouch(const FInputActionValue& value)
{
	if (bIsCrouching)
	{
		UnCrouch();
		bIsCrouching = false;
		if (GetCharacterMovement()) {
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		}
	}
}

bool AMyCharacter::GetIsCrouching() const
{
	return bIsCrouching;
}

#pragma endregion

#pragma region Zoom & Attack
// ======================  캐릭터 공격, 줌  ===========================
void AMyCharacter::PerformMeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("PerformMeleeAttack")); // 실행 확인용 로그

	if (WeaponSlot)
	{
		// TO DO : Equipped Weapon Type 별 캐스팅 분리 or 액션 분리

		ABaseWeapon* equippedWeapon = Cast<ABaseWeapon>(WeaponSlot->GetChildActor());
		//EquippedWeapon = Cast<ABaseWeapon>(Weapon->GetChildActor());
		if (equippedWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Using Weapon: %s"), *equippedWeapon->GetName());
			equippedWeapon->Attack(); // 무기의 Attack 함수 호출

			// 로깅용 => 추후 삭제 필요
			if (AFirearm* fireWeapon = Cast<AFirearm>(equippedWeapon))
			{
				LogFireAmmoState(fireWeapon);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Weapon Equipped"));
		}
	}

}

void AMyCharacter::StartZoom()
{
	ToggleFirstPerson();
	//bIsZooming = true;
	//APlayerController* PC = Cast<APlayerController>(GetController());
	//if (PC)
	//{
	//	APlayerHUD* HUD = Cast<APlayerHUD>(PC->GetHUD());
	//	if (HUD)
	//	{
	//		HUD->SetCrosshairSize(true);
	//	}
	//}
}

void AMyCharacter::StopZoom()
{
	ToggleFirstPerson();
	//bIsZooming = false;
	//APlayerController* PC = Cast<APlayerController>(GetController());
	//if (PC)
	//{
	//	APlayerHUD* HUD = Cast<APlayerHUD>(PC->GetHUD());
	//	if (HUD)
	//	{
	//		HUD->SetCrosshairSize(false);
	//	}
	//}
}

void AMyCharacter::ToggleFirstPerson()
{
	bIsFirstPerson = !bIsFirstPerson;

	if (bIsFirstPerson)
	{
		SavedCameraLocation = CameraComp->GetComponentLocation();
		SavedCameraRotation = CameraComp->GetComponentRotation();

		SpringArmComp->TargetArmLength = 0.f;
		CameraComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "head");
		CameraComp->SetRelativeLocation(FirstPersonCameraOffset);
		CameraComp->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		CameraComp->bUsePawnControlRotation = true;
	}
	else
	{
		CameraComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::SnapToTargetIncludingScale);


		SpringArmComp->TargetArmLength = 75.f;
		SpringArmComp->SetRelativeLocation(FVector(0.f, 45.f, 52.f));
		CameraComp->bUsePawnControlRotation = false;
	}
}


void AMyCharacter::TryReload()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Reload Weapon"));

	if (WeaponSlot)
	{
		// TO DO : Equipped Weapon Type 별 캐스팅 분리 or 액션 분리
		AFirearm* equippedWeapon = Cast<AFirearm>(WeaponSlot->GetChildActor());
		
		if (equippedWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Using Weapon: %s"), *equippedWeapon->GetName());

			if (ReloadSequence) // ReloadSequence는 UAnimSequence* 타입 변수입니다.
			{
				if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
				{
					UAnimMontage* MontageInstance = AnimInstance->PlaySlotAnimationAsDynamicMontage(ReloadSequence, FName("DefaultSlot"), 0.2f, 0.2f, 1.0f);
					if (MontageInstance)
					{
						float Duration = MontageInstance->GetPlayLength();
						// Duration을 활용해서 재장전 로직 후속 처리를 할 수 있습니다.
					}
				}
			}

			equippedWeapon->Reload(); // 무기의 Attack 함수 호출
			UE_LOG(LogTemp, Error, TEXT("test"));
			int32 value = equippedWeapon->GetCurrentAmmoValue();

			LogFireAmmoState(equippedWeapon);	// 로깅용 => 추후 삭제 필요
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Weapon Equipped"));
		}
	}

}

void AMyCharacter::AttachParts()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Attach Parts To Weapon"));

	if (WeaponSlot)
	{
		AFirearm* equippedWeapon = Cast<AFirearm>(WeaponSlot->GetChildActor());

		if (Magazine)
		{
			AParts* magazine = Cast<AParts>(Magazine->GetChildActor());
			equippedWeapon->EquipParts(magazine); // 무기의 Attack 함수 호출
		}
	}
}

void AMyCharacter::TryAddAmmo()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Add Ammo To Weapon"));

	if (WeaponSlot)
	{
		AFirearm* equippedWeapon = Cast<AFirearm>(WeaponSlot->GetChildActor());

		equippedWeapon->AddAmmo(30); // 무기의 Attack 함수 호출

		LogFireAmmoState(equippedWeapon);	// 로깅용 => 추후 삭제 필요
	}
}


#pragma endregion

void AMyCharacter::TryEquipMainWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Equip Main Weapon"));

}

void AMyCharacter::TryEquipSubWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Equip Sub Weapon"));

}

void AMyCharacter::TryEquipMeleeWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Equip Melee Weapon"));

}

void AMyCharacter::TryEquipThrowableWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Equip Throwable Weapon"));

}

void AMyCharacter::TryUseHealingItem()
{
	UE_LOG(LogTemp, Warning, TEXT("Use Healing item"));

	GetWorldTimerManager().SetTimer(
		HealingTimerHandle,
		this,
		&AMyCharacter::CompleteHealing,
		3.0f, // 3초 후 실행
		false
	);

	for (int i = 1; i <= 3; i++)
	{
		FTimerHandle TempHandle;
		GetWorldTimerManager().SetTimer(
			TempHandle,
			[this]() { UE_LOG(LogTemp, Warning, TEXT("healing")); },
			i,
			false
		);
	}
}

void AMyCharacter::CompleteHealing()
{
	Health = FMath::Min(Health + 20.0f, 100.0f);
	UE_LOG(LogTemp, Warning, TEXT("healing complete: %f"), Health);
}

void AMyCharacter::OnInputInventoryKey()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHexboundGameInstance* HexboundGameInstance = Cast<UHexboundGameInstance>(GameInstance);
		if (HexboundGameInstance == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cant' Find Hexbound GameInstance"));
			return;
		}

		UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
		if (UIManager == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cant' Find UIManager"));
			return;
		}

		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController);

			if (bIsShowInventory)
			{
				UIManager->HideUI(EHUDState::Inventory);
				UE_LOG(LogTemp, Warning, TEXT("Hide Inventory UI"));
				MyPlayerController->ShowCursor(false);
			}
			else
			{
				UIManager->ShowUI(EHUDState::Inventory);
				UE_LOG(LogTemp, Warning, TEXT("Show Inventory UI"));
				MyPlayerController->ShowCursor(true);
			}

			bIsShowInventory = !bIsShowInventory;
		}

	}
}

void AMyCharacter::OnInputESCKey()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHexboundGameInstance* HexboundGameInstance = Cast<UHexboundGameInstance>(GameInstance);
		if (HexboundGameInstance == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cant' Find Hexbound GameInstance"));
			return;
		}

		UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
		if (UIManager == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Cant' Find UIManager"));
			return;
		}

		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController);

			if (bIsShowPauseUI)
			{
				UIManager->HideUI(EHUDState::Pause);
				UE_LOG(LogTemp, Warning, TEXT("Hide Pause UI"));
				MyPlayerController->ShowCursor(false);
			}
			else
			{
				UIManager->ShowUI(EHUDState::Pause);
				UE_LOG(LogTemp, Warning, TEXT("Show Pause UI"));
				MyPlayerController->ShowCursor(true);
			}

			bIsShowPauseUI = !bIsShowPauseUI;
		}
	}
}

void AMyCharacter::TryPickUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Try PickUp Item"));
}

void AMyCharacter::CharacterTakeDamage(float DamageAmount)
{
	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("체력: %f"), Health);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Health : %f"), Health));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	}
	if (HUDWidget)
	{
		HUDWidget->UpdateHealth(Health, MaxHealth);
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CharacterTakeDamage(ActualDamage);

	return ActualDamage;
}

void AMyCharacter::OnPlayerDeath()
{
	// TO DO : Character Death Behavior or Show Finish Widget

}


void AMyCharacter::LogFireAmmoState(AFirearm* fireWeapon)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
		FString::Printf(TEXT("Current Ammo : %d!! %d / %d "),
			fireWeapon->GetCurrentAmmoValue(),
			fireWeapon->GetReloadedAmmoValue(),
			fireWeapon->GetMaxReloadedAmmoValue()));
}

void AMyCharacter::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
	if (HUDWidget)
	{
		HUDWidget->UpdateAmmo(CurrentAmmo, MaxAmmo);
	}
}


#pragma region Bind Mapping

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			// ======================  캐릭터 이동관련  ===========================
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Move
				);
			}
			if (PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartJump
				);
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopJump
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Look
				);
			}
			if (PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartSprint
				);
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopSprint
				);
			}
			if (PlayerController->CrouchAction)
			{
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::StartCrouch
				);
				EnhancedInput->BindAction(
					PlayerController->CrouchAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopCrouch
				);
			}

			// ======================  캐릭터 공격, 줌  ===========================
			if (PlayerController->AttackAction)
			{
				UE_LOG(LogTemp, Warning, TEXT("Binding MeleeAttackAction to PerformMeleeAttack"));
				EnhancedInput->BindAction(
					PlayerController->AttackAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::PerformMeleeAttack
				);
			}
			if (PlayerController->ZoomAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::ToggleFirstPerson
				);
			}
			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(
					PlayerController->ReloadAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::TryReload
				);
			}

			// ======================  캐릭터 무기 장착 및 아이템 사용  ===========================
			if (PlayerController->EquipMainAction)
			{
				EnhancedInput->BindAction(
					PlayerController->EquipMainAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::TryEquipMainWeapon
				);
			}

			if (PlayerController->EquipSubAction)
			{
				EnhancedInput->BindAction(
					PlayerController->EquipSubAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::TryEquipSubWeapon
				);
			}

			if (PlayerController->EquipMeleeAction)
			{
				EnhancedInput->BindAction(
					PlayerController->EquipMeleeAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::TryEquipMeleeWeapon
				);
			}

			if (PlayerController->EquipThrowableAction)
			{
				EnhancedInput->BindAction(
					PlayerController->EquipThrowableAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::TryEquipThrowableWeapon
				);
			}

			if (PlayerController->UseHealthItemAction)
			{
				EnhancedInput->BindAction(
					PlayerController->UseHealthItemAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::TryUseHealingItem
				);
			}


			if (PlayerController->DebuggingPartsAction)
			{
				EnhancedInput->BindAction(
					PlayerController->DebuggingPartsAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::AttachParts
				);
			}

			if (PlayerController->TestAddAmmoAction)
			{
				EnhancedInput->BindAction(
					PlayerController->TestAddAmmoAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::TryAddAmmo
				);
			}

			// ====================== 설정, 인벤토리, 아이템 상호작용  ===========================
			if (PlayerController->InputInventoryAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InputInventoryAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::OnInputInventoryKey
				);
			}

			if (PlayerController->InputESCAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InputESCAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::OnInputESCKey
				);
			}

			if (PlayerController->InputPickUpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->InputPickUpAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::TryPickUp
				);
			}
		}
	}
}


#pragma endregion


