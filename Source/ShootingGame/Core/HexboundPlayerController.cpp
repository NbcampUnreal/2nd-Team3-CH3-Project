#include "HexboundPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Managers/UIManager.h"

AHexboundPlayerController::AHexboundPlayerController()
{

}

void AHexboundPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AHexboundPlayerController::BeginPlay()
{
	Super::BeginPlay();

}


void AHexboundPlayerController::ShowCursor(bool isShow)
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
