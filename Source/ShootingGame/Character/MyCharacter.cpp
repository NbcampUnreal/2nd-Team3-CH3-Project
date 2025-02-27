

#include "Character/MyCharacter.h"
#include "Character/MyPlayerController.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/MeleeWeapon.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ChildActorComponent.h"

#include "Core/HexboundGameInstance.h"
#include "Managers/UIManager.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//������ ��
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	//ī�޶�
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	//�ӵ� ����
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

	// Weapon ChildActorComponent ����
	WeaponSlot = CreateDefaultSubobject<UChildActorComponent>(TEXT("WeaponSlot"));
	WeaponSlot->SetupAttachment(GetMesh(), TEXT("RightHand")); // Skeletal Mesh�� Slot �̸�
	//WeaponSlot->SetChildActorClass(DefaultWeaponClass); // �⺻ ���� Ŭ���� ����
}

//void AMyCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//
//	// �⺻ ���� ����
//	if (DefaultWeaponClass)
//	{
//		EquippedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(DefaultWeaponClass);
//		if (EquippedWeapon)
//		{
//			EquippedWeapon->AttachWeaponToCharacter(this);
//		}
//	}
//}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CameraComp)
	{
		float TargetFOV = bIsZooming ? ZoomedFOV : DefaultFOV;
		float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
		CameraComp->SetFieldOfView(NewFOV);
	}
}

#pragma region Character Movement
// ======================  ĳ���� �̵�����  ===========================
void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller || !CameraComp) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	if (MoveInput.IsNearlyZero()) return;

	// ī�޶��� ȸ�� ���� ��������
	const FRotator CameraRotation = CameraComp->GetComponentRotation();
	const FRotator YawRotation(0.f, CameraRotation.Yaw, 0.f); // Pitch�� Roll ����, Yaw�� ����

	// ĳ���Ͱ� �׻� ī�޶� ������ ���� ����
	SetActorRotation(YawRotation);

	// ī�޶� ������ ��/�� ����
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// ī�޶� ������ ��/�� ����
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// �̵� �Է� ����
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
// ======================  ĳ���� ����, ��  ===========================
void AMyCharacter::PerformMeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("PerformMeleeAttack")); // ���� Ȯ�ο� �α�

	if (WeaponSlot)
	{
		// TO DO : Equipped Weapon Type �� ĳ���� �и� or �׼� �и�

		ABaseWeapon* equippedWeapon = Cast<ABaseWeapon>(WeaponSlot->GetChildActor());
		//EquippedWeapon = Cast<ABaseWeapon>(Weapon->GetChildActor());
		if (equippedWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Using Weapon: %s"), *equippedWeapon->GetName());
			equippedWeapon->Attack(); // ������ Attack �Լ� ȣ��

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Weapon Equipped"));
		}
	}

}

void AMyCharacter::StartZoom()
{
	bIsZooming = true;
}

void AMyCharacter::StopZoom()
{
	bIsZooming = false;
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
		3.0f, // 3�� �� ����
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
	Health = FMath::Max(Health, 0.0f);
	UE_LOG(LogTemp, Warning, TEXT("ü��: %f"), Health);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Current Health : %f"), Health));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
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

#pragma region Bind Mapping

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			// ======================  ĳ���� �̵�����  ===========================
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

			// ======================  ĳ���� ����, ��  ===========================
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
					&AMyCharacter::StartZoom
				);

				EnhancedInput->BindAction(
					PlayerController->ZoomAction,
					ETriggerEvent::Completed,
					this,
					&AMyCharacter::StopZoom
				);
			}

			// ======================  ĳ���� ���� ���� �� ������ ���  ===========================
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

			// ====================== ����, �κ��丮, ������ ��ȣ�ۿ�  ===========================
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


