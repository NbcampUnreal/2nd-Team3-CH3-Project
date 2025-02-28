#include "UI/SettingWidget.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Core/HexboundGameInstance.h"

void USettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();

		// 버튼이 nullptr이 아닐 때만 바인딩
		if (ButtonAudioSetting)
		{
			ButtonAudioSetting->OnClicked.AddDynamic(this, &USettingWidget::OnClickedAudioSetting);
		}
		if (ButtonVideoSetting)
		{
			ButtonVideoSetting->OnClicked.AddDynamic(this, &USettingWidget::OnClickedVideoSetting);
		}
		if (ButtonKeyMapping)
		{
			ButtonKeyMapping->OnClicked.AddDynamic(this, &USettingWidget::OnClickedKeyMapping);
		}
		if (ButtonBack)
		{
			ButtonBack->OnClicked.AddDynamic(this, &USettingWidget::OnClickedBack);
		}

		isBinding = true;
	}
}

void USettingWidget::OnClickedAudioSetting()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Audio Setting Button")));

	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::AudioSetting);
	}
}

void USettingWidget::OnClickedVideoSetting()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Video Setting Button")));

	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::VideoSetting);
	}
}

void USettingWidget::OnClickedKeyMapping()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked KeyMapping Button")));

	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::KeyMapping);
	}
}

void USettingWidget::OnClickedBack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Back Button")));

	if (uiManager)
	{
		UGameInstance* gameInstance = GetGameInstance();
		UHexboundGameInstance* hexboundInstance = Cast<UHexboundGameInstance>(gameInstance);
		if (hexboundInstance)
		{
			switch (hexboundInstance->GetCurrentLevel())
			{
			case ELevel::MenuLevel:
				uiManager->SetUIState(EHUDState::MainMenu);
				break;
			case ELevel::DummyLevel:
				uiManager->HideUI(EHUDState::Setting);
				uiManager->ShowUI(EHUDState::Pause);
				break;
			default:
				break;
			}
		}
	}
}
