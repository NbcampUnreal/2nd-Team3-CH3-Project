

#include "Character/MyCharacter.h"
#include "Character/MyPlayerController.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/Firearm.h"
#include "Weapon/MeleeWeapon.h"
#include "Weapon/Parts.h"
#include "Weapon/MainWeapon.h"
#include "Weapon/SubWeapon.h"
#include "Weapon/ThrowableWeapon.h"
#include "Weapon/Magazine.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ChildActorComponent.h"
#include "UI/HexPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Core/HexboundGameInstance.h"
#include "Managers/UIManager.h"
#include "UI/InGame.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

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

	MainWeaponActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("MainWeapon"));
	MainWeaponActor->SetupAttachment(GetMesh(), TEXT("MainWeaponSocket"));
	MainWeaponActor->SetChildActorClass(AMainWeapon::StaticClass());

	SubWeaponActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("SubWeapon"));
	SubWeaponActor->SetupAttachment(GetMesh(), TEXT("SubWeaponSocket"));
	SubWeaponActor->SetChildActorClass(ASubWeapon::StaticClass());

	MeleeWeaponActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("MeleeWeapon"));
	MeleeWeaponActor->SetupAttachment(GetMesh(), TEXT("MeleeWeaponSocket"));
	MeleeWeaponActor->SetChildActorClass(AMeleeWeapon::StaticClass());

	ThrowableWeaponActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("ThrowableWeapon"));
	ThrowableWeaponActor->SetupAttachment(GetMesh(), TEXT("ThrowableWeaponSocket"));
	ThrowableWeaponActor->SetChildActorClass(AThrowableWeapon::StaticClass());


	// 탄창
	MainWeaponMagz = CreateDefaultSubobject<UChildActorComponent>(TEXT("MainWeaponMagazine"));
	SubWeaponMagz = CreateDefaultSubobject<UChildActorComponent>(TEXT("SubWeaponMagazine"));

	InitUIImage = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Weapons/Images/T_FistImage.T_FistImage"));
}

void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

    UE_LOG(LogTemp, Warning, TEXT("current Level: %s"), *CurrentLevelName);

    // MenuLevel이면 HUD를 숨김
    if (CurrentLevelName == "MenuLevel")
    {
        UE_LOG(LogTemp, Warning, TEXT("MenuLevel HUD"));

        if (HUDWidget)
        {
            HUDWidget->RemoveFromViewport();  // HUD 삭제
        }
    }
    else
    {
        // MenuLevel이 아닐 때만 HUD 표시
        if (HUDWidgetClass)
        {
            HUDWidget = CreateWidget<UHexPlayerHUD>(GetWorld(), HUDWidgetClass);
            if (HUDWidget)
            {
                HUDWidget->AddToViewport();
                HUDWidget->UpdateHealth(Health, MaxHealth);
            }
        }
    }

	OurGameInstance = Cast<UHexboundGameInstance>(GetGameInstance());

	// 시작시 총기 없음
	MainWeaponActor->SetVisibility(false);
	SubWeaponActor->SetVisibility(false);
	MeleeWeaponActor->SetVisibility(false);
	ThrowableWeaponActor->SetVisibility(false);

	//총기 UI 초기화
	InitializingUI();
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
		if (AFirearm* equippedWeapon = Cast<AFirearm>(GetCurrentWeapon()))
		{
			//EquippedWeapon = Cast<ABaseWeapon>(Weapon->GetChildActor());
			if (equippedWeapon)
			{
				UE_LOG(LogTemp, Warning, TEXT("Using Weapon: %s"), *equippedWeapon->GetName());
				equippedWeapon->Attack(); // 무기의 Attack 함수 호출

				// 로깅용 => 추후 삭제 필요
				LogFireAmmoState(equippedWeapon);

			}
		}

		else if (AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(GetCurrentWeapon()))
		{
			MeleeWeapon->Attack();
		}

		else if (AThrowableWeapon* ThrowableWeapon = Cast<AThrowableWeapon>(GetCurrentWeapon()))
		{
			if (ThrowableWeapon->bIsInHand)
			{
				ThrowableWeapon->Throw();
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

	if (AFirearm* equippedWeapon = Cast<AFirearm>(GetCurrentWeapon()))
	{
		if (equippedWeapon)
		{
			equippedWeapon->SetReloadCondition();

			if (equippedWeapon->GetReloadCondition())
			{
				UE_LOG(LogTemp, Warning, TEXT("Using Weapon: %s"), *equippedWeapon->GetName());

				StartReload();  //bIsReloading = true (재장전 시작)

				if (ReloadSequence)  //재장전 애니메이션이 존재하는 경우
				{
					if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
					{
						UAnimMontage* MontageInstance = AnimInstance->PlaySlotAnimationAsDynamicMontage(
							ReloadSequence,
							FName("DefaultSlot"),
							0.25f,  //Blending 시작 시간 (최적화)
							0.25f, //Blending 종료 시간 (T-포즈 최소화)
							1.0f
						);

						if (MontageInstance)
						{
							float Duration = MontageInstance->GetPlayLength();
							//float AdjustedTime = FMath::Max(Duration - 0.1f, 0.05f); 

							//GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AMyCharacter::StartReload, 0.1f, false);
							GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AMyCharacter::EndReload, Duration, false);
						}
					}
				}

				equippedWeapon->Reload();
				LogFireAmmoState(equippedWeapon);
			}
		}
	}

	else if (AThrowableWeapon* ThrowableWeapon = Cast<AThrowableWeapon>(GetCurrentWeapon()))
	{
		ThrowableWeapon->Spawn();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Weapon Equipped"));
	}

}

void AMyCharacter::AttachParts()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Attach Parts To Weapon"));

	AFirearm* equippedWeapon = Cast<AFirearm>(GetCurrentWeapon());

	if (MainWeaponMagz && MainWeaponActor->IsVisible() && !bIsMainWeaponMagExist)
	{
		UClass* MainMagClassptr = MainWeaponMagz->GetChildActorClass();
		MainWeaponMagActor = GetWorld()->SpawnActor<AMagazine>(MainMagClassptr);
		equippedWeapon->EquipParts(MainWeaponMagActor); // 무기의 EquipParts 함수 호출
		bIsMainWeaponMagExist = true;
	}

	else if (SubWeaponMagz && SubWeaponActor->IsVisible() && !bIsSubWeaponMagExist)
	{
		UClass* SubMagClassptr = SubWeaponMagz->GetChildActorClass();
		SubWeaponMagActor = GetWorld()->SpawnActor<AMagazine>(SubMagClassptr);
		equippedWeapon->EquipParts(SubWeaponMagActor); // 무기의 EquipParts 함수 호출
		bIsSubWeaponMagExist = true;
	}
}

void AMyCharacter::TryAddAmmo()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Add Ammo To Weapon"));

	if (AFirearm* equippedWeapon = Cast<AFirearm>(GetCurrentWeapon()))
	{
		equippedWeapon->AddAmmo(30); // 무기의 Attack 함수 호출

		LogFireAmmoState(equippedWeapon);	// 로깅용 => 추후 삭제 필요
	}

	else if (AThrowableWeapon* ThrowableWeapon = Cast<AThrowableWeapon>(GetCurrentWeapon()))
	{
		ThrowableWeapon->AddQuantity(1);
	}
}


#pragma endregion

void AMyCharacter::InitializingUI()
{
	UIManagerForInitUI  = GetGameInstance()->GetSubsystem<UUIManager>();
	if (UIManagerForInitUI)
	{
		UUserWidget* InGameWidgetInstance = UIManagerForInitUI->WidgetInstances.FindRef(EHUDState::InGameBase);
		if (UInGame* InGameWidget = Cast<UInGame>(InGameWidgetInstance))
		{
			InGameWidget->PrintCurrentWeapon("Fist", InitUIImage);
		}
	}
}

ABaseWeapon* AMyCharacter::GetCurrentWeapon() const
{
	ABaseWeapon* equippedWeapon = nullptr;

	if (MainWeaponActor->IsVisible())
	{
		equippedWeapon = Cast<ABaseWeapon>(MainWeaponActor->GetChildActor());
	}

	else if (SubWeaponActor->IsVisible())
	{
		equippedWeapon = Cast<ABaseWeapon>(SubWeaponActor->GetChildActor());
	}

	else if (MeleeWeaponActor->IsVisible())
	{
		equippedWeapon = Cast<ABaseWeapon>(MeleeWeaponActor->GetChildActor());
	}

	else if (ThrowableWeaponActor->IsVisible())
	{
		equippedWeapon = Cast<ABaseWeapon>(ThrowableWeaponActor->GetChildActor());
	}

	return equippedWeapon;
}

void AMyCharacter::PlayWeaponSwapAnimation(TFunction<void()> OnAnimationEnd)
{
	if (bIsSwappingWeapon) return; // 이미 무기 교체 중이면 중복 실행 방지

	bIsSwappingWeapon = true;

	UE_LOG(LogTemp, Warning, TEXT("Try weapon swap animation"));

	//블루프린트에서 bIsSwappingWeapon 값 변경
	SetWeaponSwapState(true);

	if (WeaponSwapAnimation)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			UAnimMontage* MontageInstance = AnimInstance->PlaySlotAnimationAsDynamicMontage(
				WeaponSwapAnimation,
				FName("WeaponSwapSlot"),
				0.2f,
				0.2f,
				1.0f
			);

			if (MontageInstance)
			{
				float Duration = MontageInstance->GetPlayLength();

				FTimerDelegate TimerDelegate;
				TimerDelegate.BindLambda([this, OnAnimationEnd]() {
					FinishWeaponSwap();
					OnAnimationEnd();
					});

				GetWorldTimerManager().SetTimer(
					SwapWeaponTimerHandle,
					TimerDelegate,
					Duration,
					false
				);
			}
		}
	}
	else
	{
		FinishWeaponSwap();
		OnAnimationEnd();
	}
}

void AMyCharacter::FinishWeaponSwap()
{
	bIsSwappingWeapon = false;

	SetWeaponSwapState(false);
}

void AMyCharacter::TryEquipMainWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Equip Main Weapon"));
	PlayWeaponSwapAnimation([this]() {
		SubWeaponActor->SetVisibility(false);
		if (bIsSubWeaponMagExist)
		{
			SubWeaponMagActor->SetHidden(true);
		}
		MeleeWeaponActor->SetVisibility(false);
		ThrowableWeaponActor->SetVisibility(false);

		MainWeaponActor->SetVisibility(true);
		if (bIsMainWeaponMagExist)
		{
			MainWeaponMagActor->SetHidden(false);
		}

		AMainWeapon* Weapon = Cast<AMainWeapon>(MainWeaponActor->GetChildActor());
		Weapon->UpdateWeaponUI();
		});
}

void AMyCharacter::TryEquipSubWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Equip Sub Weapon"));

	PlayWeaponSwapAnimation([this]() {
		MainWeaponActor->SetVisibility(false);
		if (bIsMainWeaponMagExist)
		{
			MainWeaponMagActor->SetHidden(true);
		}
		MeleeWeaponActor->SetVisibility(false);
		ThrowableWeaponActor->SetVisibility(false);

		SubWeaponActor->SetVisibility(true);
		if (bIsSubWeaponMagExist)
		{
			SubWeaponMagActor->SetHidden(false);
		}

		ASubWeapon* Weapon = Cast<ASubWeapon>(SubWeaponActor->GetChildActor());
		Weapon->UpdateWeaponUI();
		});
}

void AMyCharacter::TryEquipMeleeWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Equip Melee Weapon"));

	PlayWeaponSwapAnimation([this]() {
		MainWeaponActor->SetVisibility(false);
		if (bIsMainWeaponMagExist)
		{
			MainWeaponMagActor->SetHidden(true);
		}
		SubWeaponActor->SetVisibility(false);
		if (bIsSubWeaponMagExist)
		{
			SubWeaponMagActor->SetHidden(true);
		}
		ThrowableWeaponActor->SetVisibility(false);

		MeleeWeaponActor->SetVisibility(true);

		AMeleeWeapon* Weapon = Cast<AMeleeWeapon>(MeleeWeaponActor->GetChildActor());
		Weapon->UpdateWeaponUI();
		});
}

void AMyCharacter::TryEquipThrowableWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Equip Throwable Weapon"));

	PlayWeaponSwapAnimation([this]() {
		MainWeaponActor->SetVisibility(false);
		if (bIsMainWeaponMagExist)
		{
			MainWeaponMagActor->SetHidden(true);
		}
		SubWeaponActor->SetVisibility(false);
		if (bIsSubWeaponMagExist)
		{
			SubWeaponMagActor->SetHidden(true);
		}
		MeleeWeaponActor->SetVisibility(false);

		ThrowableWeaponActor->SetVisibility(true);

		AThrowableWeapon* Weapon = Cast<AThrowableWeapon>(ThrowableWeaponActor->GetChildActor());
		Weapon->UpdateWeaponUI();
		});
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

//void AMyCharacter::CharacterTakeDamage(float DamageAmount)
//{
//	Health -= DamageAmount;
//	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
//	UE_LOG(LogTemp, Warning, TEXT("체력: %f"), Health);
//	if (GEngine)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Health : %f"), Health));
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("NULL"));
//	}
//	if (HUDWidget)
//	{
//		HUDWidget->UpdateHealth(Health, MaxHealth);
//	}
//}

void AMyCharacter::CharacterTakeDamage(float DamageAmount)
{
	Health -= DamageAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("체력: %f"), Health);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Health : %f"), Health));
	}

	if (HUDWidget)
	{
		HUDWidget->UpdateHealth(Health, MaxHealth);
	}

	//피격 애니메이션 실행 (HitReactionSequence 사용)
	if (HitReactionSequence && Health > 0.0f)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(
				HitReactionSequence,
				FName("HitReactionSlot"),
				0.01f,  // Blend In
				0.01f,  // Blend Out
				0.1f   // 재생 속도
			);
		}
	}
	bIsHitReacting = true;
	GetWorldTimerManager().SetTimer(HitResetTimerHandle, this, &AMyCharacter::ResetHitState, 1.0f, false);
	//체력이 0이면 사망 처리
	if (Health <= 0.0f)
	{
		OnPlayerDeath();
	}
}

bool AMyCharacter::GetIsHitReacting() const
{
	return bIsHitReacting;
}

void AMyCharacter::ResetHitState()
{
	bIsHitReacting = false;
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
	UE_LOG(LogTemp, Warning, TEXT("character dead!"));

	bIsDead = true;

	// 캐릭터가 죽었을 때, 컨트롤러 입력 막기
	AController* PlayerController = GetController();
	if (PlayerController)
	{
		DisableInput(Cast<APlayerController>(PlayerController));
	}

	// 사망 애니메이션 실행 (DeathSequence 사용)
	if (DeathSequence)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->PlaySlotAnimationAsDynamicMontage(
				DeathSequence,
				FName("FullBodySlot"),
				0.2f,  // Blend In
				0.2f,  // Blend Out
				1.0f   // 재생 속도
			);
		}
	}

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	// 일정 시간이 지난 후 캐릭터 제거
	GetWorldTimerManager().SetTimer(
		DeathTimerHandle,
		this,
		&AMyCharacter::DestroyCharacter,
		3.0f, // 3초 후 제거
		false
	);
}

bool AMyCharacter::GetIsDead() const
{
	return bIsDead;
}

void AMyCharacter::DestroyCharacter()
{
	Destroy();
}

void AMyCharacter::LogFireAmmoState(AFirearm* fireWeapon)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
		FString::Printf(TEXT("Current Ammo : %d!! %d / %d "),
			fireWeapon->GetCurrentAmmoValue(),
			fireWeapon->GetReloadedAmmoValue(),
			fireWeapon->GetMaxReloadedAmmoValue()));
}


void AMyCharacter::StartReload()
{
	bIsReloading = true;
	// AnimBP에서는 Event Blueprint Update Animation에서 이 값(bIsReloading)을 읽어
	// Blend Poses by Bool 노드를 통해 재장전 상태에 따른 상체 애니메이션 전환을 처리합니다.
}

void AMyCharacter::EndReload()
{
	bIsReloading = false;
	// 재장전 애니메이션이 끝난 후, 일반 모션으로 돌아갈 수 있도록 상태 전환 처리
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


