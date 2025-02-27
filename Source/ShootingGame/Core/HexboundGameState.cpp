#include "HexboundGameState.h"
#include "HexboundGameInstance.h"
#include "HexboundPlayerController.h"
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
		AHexboundPlayerController* HexBoundPlayerController = Cast<AHexboundPlayerController>(PlayerController);
		hexBoundPlayerController = HexBoundPlayerController;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Set HexboundPlayerController At UIMangaer"));
	}
}

void AHexboundGameState::OnGameStart()
{
	// TO DO : Level Load ( Main Level )
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
				if(hexBoundPlayerController)hexBoundPlayerController->ShowCursor(true);
				break;
			case ELevel::MainLevel:
				UIManager->SetUIState(EHUDState::InGameBase);
				if (hexBoundPlayerController)hexBoundPlayerController->ShowCursor(false);
				break;
			default:
				break;
			}

		}
	}
}
//
//void AHexboundGameState::OpenLevel(ELevel level)
//{
//	if (UGameInstance* GameInstance = GetGameInstance())
//	{
//		UHexboundGameInstance* HexboundGameInstance = Cast<UHexboundGameInstance>(GameInstance);
//		HexboundGameInstance->SetCurrentLevel(level);
//	}
//
//	FName levelName = FName(*StaticEnum<ELevel>()->GetNameStringByValue(static_cast<int64>(level)));
//	UGameplayStatics::OpenLevel(GetWorld(), levelName);
//}
