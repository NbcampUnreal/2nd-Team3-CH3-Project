#include "UI/MainMenu.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"
#include "Core/HexboundGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();

		// 버튼이 nullptr이 아닐 때만 바인딩
		if (GameStartButton)
		{
			GameStartButton->OnClicked.AddDynamic(this, &UMainMenu::OnClickedGameStart);
		}
		if (SettingButton)
		{
			SettingButton->OnClicked.AddDynamic(this, &UMainMenu::OnClickedSetting);
		}
		if (ArchievementsButton)
		{
			ArchievementsButton->OnClicked.AddDynamic(this, &UMainMenu::OnClickedAchievements);
		}
		if (ExitButton)
		{
			ExitButton->OnClicked.AddDynamic(this, &UMainMenu::OnClickedExit);
		}

		isBinding = true;
	}
}

void UMainMenu::OnClickedGameStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked GameStart Button")));

	if (uiManager)
	{
		uiManager->HideAllUI();
		if (UGameInstance* gameInstance = GetGameInstance())
		{
			UHexboundGameInstance* hexboundGameInstance = Cast<UHexboundGameInstance>(gameInstance);
			if (hexboundGameInstance) hexboundGameInstance->OpenLevel(ELevel::MainLevel);
		}

	}
}

void UMainMenu::OnClickedSetting()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Setting Button")));

	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::Setting);
	}
}

void UMainMenu::OnClickedAchievements()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Achievements Button")));

	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::Archivements);
	}
}

void UMainMenu::OnClickedExit()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Exit Button")));

	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}
