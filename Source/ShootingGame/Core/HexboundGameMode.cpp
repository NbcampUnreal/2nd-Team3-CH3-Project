#include "HexboundGameMode.h"

#include "Default/DefaultCharacter.h"
#include "Core/HexboundGameInstance.h"
#include "Core/HexboundGameState.h"
#include "Core/HexboundPlayerController.h"
#include "Managers/UIManager.h"

AHexboundGameMode::AHexboundGameMode()
{
	// Project Settings > Maps & Modes -> BP 로 감싼 후 작업 

	DefaultPawnClass = ADefaultCharacter::StaticClass();
	//HUDClass = AHexboundHUD::StaticClass();
	PlayerControllerClass = AHexboundPlayerController::StaticClass();
	GameStateClass = AHexboundGameState::StaticClass();
	//PlayerStateClass = AHexboundPlaerStateClass();
	//SpectatorClass = AHexboundSpectatorClass::StaticClass();
}

void AHexboundGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

}
