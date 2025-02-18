#include "HexboundGameState.h"
#include "Managers/UIManager.h"

AHexboundGameState::AHexboundGameState()
{

}

void AHexboundGameState::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UUIManager* UIManager = Cast<UUIManager>(GameInstance);
		UIManager->ShowMainUI();
		UIManager->ShowSettingUI();
	}

}
