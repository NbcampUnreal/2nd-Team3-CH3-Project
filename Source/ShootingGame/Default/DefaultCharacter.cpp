// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Item/Item.h"
#include "Components/WidgetComponent.h"

#include "Core/HexboundGameInstance.h"
#include "Core/HexboundPlayerController.h"
#include "Managers/UIManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// Sets default values
ADefaultCharacter::ADefaultCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	MaxHealth = 100.0f;
	Health = MaxHealth;
	Item = nullptr;

}

float ADefaultCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player Health decreased to: %f"), Health));

	if (Health <= 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player Die!!")));
	}

	return ActualDamage;
}

void ADefaultCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADefaultCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADefaultCharacter::OnInputPickupKey()
{

	//// 겹친 액터들을 저장할 배열
	//TArray<AActor*> OverlappingActors;

	//// GetOverlappingActors를 호출하여 충돌된 액터들을 배열에 저장
	////(TArray<AActor*>& OutOverlappingActors, TSubclassOf<AActor> ClassFilter)
	//GetOverlappingActors(OverlappingActors, AItem::StaticClass());

	//if (OverlappingActors.Num() > 0)
	//{
	//	AActor* FirstActor = OverlappingActors[0]; // 첫 번째 액터 가져오기

	//	AItem* FirstItem = Cast<AItem>(OverlappingActors[0]);

	//	if (FirstItem)
	//	{
	//		//FirstItem->GetHud()->SetVisibility(false);
	//		//FirstItem->Destroy();  // 아이템 삭제
	//		Item = FirstItem;
	//	}

	//	for (int32 i = 1; i < OverlappingActors.Num(); ++i)
	//	{
	//		AItem* Temp = Cast<AItem>(OverlappingActors[i]);
	//		if (Temp && Temp->GetHud())
	//		{
	//			// HUD를 활성화
	//			Temp->GetHud()->SetVisibility(true);
	//		}
	//	}
	//}
}

void ADefaultCharacter::OnInputInventoryKey()
{
	/*if (UGameInstance* GameInstance = GetGameInstance())
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
			AHexboundPlayerController* HexBoundPlayerController = Cast<AHexboundPlayerController>(PlayerController);

			if (bIsShowInventory)
			{
				UIManager->HideUI(EHUDState::Inventory);
				UE_LOG(LogTemp, Warning, TEXT("Hide Inventory UI"));
				HexBoundPlayerController->ShowCursor(false);
			}
			else
			{
				UIManager->ShowUI(EHUDState::Inventory);
				UE_LOG(LogTemp, Warning, TEXT("Show Inventory UI"));
				HexBoundPlayerController->ShowCursor(true);
			}

			bIsShowInventory = !bIsShowInventory;
		}
		
	}*/
}

void ADefaultCharacter::OnInputESCKey()
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
			AHexboundPlayerController* HexBoundPlayerController = Cast<AHexboundPlayerController>(PlayerController);

			if (bIsShowPauseUI)
			{
				UIManager->HideUI(EHUDState::Pause);
				UE_LOG(LogTemp, Warning, TEXT("Hide Pause UI"));
				HexBoundPlayerController->ShowCursor(false);
			}
			else
			{
				UIManager->ShowUI(EHUDState::Pause);
				UE_LOG(LogTemp, Warning, TEXT("Show Pause UI"));
				HexBoundPlayerController->ShowCursor(true);
			}

			bIsShowPauseUI = !bIsShowPauseUI;
		}
	}
}


void ADefaultCharacter::NotifyControllerChanged()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADefaultCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADefaultCharacter::Look);

		// Pickup
		//EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Started, this, &ADefaultCharacter::OnInputPickupKey);

		// Inventory
		//EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ADefaultCharacter::OnInputInventoryKey);

		// ESC
		EnhancedInputComponent->BindAction(SettingAction, ETriggerEvent::Started, this, &ADefaultCharacter::OnInputESCKey);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

