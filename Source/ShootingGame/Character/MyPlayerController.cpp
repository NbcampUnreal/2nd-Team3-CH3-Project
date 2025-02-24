#include "Character/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	CrouchAction(nullptr)
{

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	if (MeleeAttackAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeAttackAction is set"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MeleeAttackAction is NULL"));
	}
	if (UseHealthItemAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("UseHealthItemAction is set"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UseHealthItemAction is NULL"));
	}
	if (ZoomAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("ZoomAction is set"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ZoomAction is NULL"));
	}
}


