

#include "Character/MyCharacter.h"
#include "Character/MyPlayerController.h"
#include "Weapon/BaseWeapon.h"
#include "Weapon/MeleeWeapon.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = true;

	if (CameraComp)
	{
		DefaultFOV = CameraComp->FieldOfView;
	}
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

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
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
			if (PlayerController->MeleeAttackAction)
			{
				UE_LOG(LogTemp, Warning, TEXT("Binding MeleeAttackAction to PerformMeleeAttack"));
				EnhancedInput->BindAction(
					PlayerController->MeleeAttackAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::PerformMeleeAttack
				);
			}
			if (PlayerController->UseHealthItemAction)
			{
				EnhancedInput->BindAction(
					PlayerController->UseHealthItemAction,
					ETriggerEvent::Started,
					this,
					&AMyCharacter::StartHealing
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
		}
	}
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller || !CameraComp) return;

	const FVector2D MoveInput = value.Get<FVector2D>();

	// ī�޶��� ȸ�� ���� ��������
	const FRotator CameraRotation = CameraComp->GetComponentRotation();
	const FRotator YawRotation(0.f, CameraRotation.Yaw, 0.f); // Pitch�� Roll�� �����ϰ� Yaw�� ���

	// ī�޶� ������ ��/�� ����
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// ī�޶� ������ ��/�� ����
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// �̵� �Է� ���� (ī�޶� ���� ����)
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(ForwardDirection, MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(RightDirection, MoveInput.Y);
	}

	/*const FVector2D MoveInput = value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}*/
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
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
	}
}
void AMyCharacter::StopCrouch(const FInputActionValue& value)
{
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}

void AMyCharacter::TakeDamage(float DamageAmount)
{
	Health -= DamageAmount;
	Health = FMath::Max(Health, 0.0f);
	UE_LOG(LogTemp, Warning, TEXT("ü��: %f"), Health);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ü��: %f"), Health));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	}
}

void AMyCharacter::PerformMeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("PerformMeleeAttack")); // ���� Ȯ�ο� �α�

	if (EquippedWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Using"));
		EquippedWeapon->Attack();

		// �ڱ� �ڽſ��� ������ ���� (�׽�Ʈ��)
		TakeDamage(EquippedWeapon->GetDamageValue());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Weapon"));
	}
	if (EquippedWeapon)
	{
		EquippedWeapon->Attack();

		// �ڱ� �ڽſ��� ������ ���� (�׽�Ʈ��)
		TakeDamage(EquippedWeapon->GetDamageValue());
	}
	UE_LOG(LogTemp, Warning, TEXT("���� ���� %f damage!"), MeleeDamage);
	TakeDamage(MeleeDamage);
}

void AMyCharacter::StartHealing()
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

void AMyCharacter::StartZoom()
{
	bIsZooming = true;
}

void AMyCharacter::StopZoom()
{
	bIsZooming = false;
}