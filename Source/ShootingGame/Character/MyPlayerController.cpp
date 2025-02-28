#include "Character/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	CrouchAction(nullptr),

	AttackAction(nullptr),
	ZoomAction(nullptr),
	ReloadAction(nullptr),
	DebuggingPartsAction(nullptr),	// 디버그용으로 Parts 붙여보기 (0)
	TestAddAmmoAction(nullptr),		// 테스트로 총알 추가 (9)

	EquipMainAction(nullptr),
	EquipSubAction(nullptr),
	EquipMeleeAction(nullptr),
	EquipThrowableAction(nullptr),
	UseHealthItemAction(nullptr),

	InputInventoryAction(nullptr),
	InputESCAction(nullptr),
	InputPickUpAction(nullptr)
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

	if (AttackAction)
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

void AMyPlayerController::ShowCursor(bool isShow)
{
	//SetActorHiddenInGame(isShow);
	bShowMouseCursor = isShow;

	if (isShow)
	{
		SetInputMode(FInputModeUIOnly());
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
	}
}


