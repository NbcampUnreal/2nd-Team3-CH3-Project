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

	// TO DO : UI�� �ִ� ȭ���϶� Player �Ⱥ��̰� �ϴ¹� ã�� - ������
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
