﻿#include "HexboundGameState.h"
#include "HexboundGameInstance.h"
#include "Character/MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Managers/UIManager.h"
#include "Kismet/GameplayStatics.h"

AHexboundGameState::AHexboundGameState()
{

}

void AHexboundGameState::BeginPlay()
{
	Super::BeginPlay();

	Init();

	OnGameStart();
}

void AHexboundGameState::Init()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController);
		myPlayerController = MyPlayerController;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Set MyPlayerController At UIMangaer"));
	}
}

void AHexboundGameState::OnGameStart()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UHexboundGameInstance* HexboundGameInstance = Cast<UHexboundGameInstance>(GameInstance);
		if (HexboundGameInstance)
		{
			UUIManager* UIManager = HexboundGameInstance->GetSubsystem<UUIManager>();

			switch (HexboundGameInstance->GetCurrentLevel())
			{
			case ELevel::MenuLevel:
				UIManager->SetUIState(EHUDState::MainMenu);
				if (myPlayerController) myPlayerController->ShowCursor(true);
				break;
			case ELevel::DummyLevel:
				UIManager->SetUIState(EHUDState::InGameBase);
				if (myPlayerController) myPlayerController->ShowCursor(false);
				break;
			default:
				break;
			}

		}
	}
}

