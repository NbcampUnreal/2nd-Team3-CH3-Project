

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
//	// 기본 무기 장착
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

	// 카메라의 회전 정보 가져오기
	const FRotator CameraRotation = CameraComp->GetComponentRotation();
	const FRotator YawRotation(0.f, CameraRotation.Yaw, 0.f); // Pitch와 Roll을 제거하고 Yaw만 사용

	// 카메라 기준의 전/후 방향
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 카메라 기준의 좌/우 방향
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// 이동 입력 적용 (카메라 방향 기준)
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
	UE_LOG(LogTemp, Warning, TEXT("체력: %f"), Health);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("체력: %f"), Health));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NULL"));
	}
}

void AMyCharacter::PerformMeleeAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("PerformMeleeAttack")); // 실행 확인용 로그

	if (EquippedWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Using"));
		EquippedWeapon->Attack();

		// 자기 자신에게 데미지 적용 (테스트용)
		TakeDamage(EquippedWeapon->GetDamageValue());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Weapon"));
	}
	if (EquippedWeapon)
	{
		EquippedWeapon->Attack();

		// 자기 자신에게 데미지 적용 (테스트용)
		TakeDamage(EquippedWeapon->GetDamageValue());
	}
	UE_LOG(LogTemp, Warning, TEXT("근접 공격 %f damage!"), MeleeDamage);
	TakeDamage(MeleeDamage);
}

void AMyCharacter::StartHealing()
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

void AMyCharacter::StartZoom()
{
	bIsZooming = true;
}

void AMyCharacter::StopZoom()
{
	bIsZooming = false;
}