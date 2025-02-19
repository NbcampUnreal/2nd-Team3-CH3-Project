#include "HexboundPlayerController.h"
#include "Blueprint/UserWidget.h"

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

	// TO DO : UI만 있는 화면일때 Player 안보이게 하는법 찾기 - 한정혁
	//SetActorHiddenInGame(true);

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
