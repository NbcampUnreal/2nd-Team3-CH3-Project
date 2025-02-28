#include "UI/AudioSettingWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Managers/UIManager.h"
#include "Core/HexboundGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UAudioSettingWidget::NativeConstruct()
{
	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();
		hexboundGameInstance = Cast<UHexboundGameInstance>(GetGameInstance());

		if (hexboundGameInstance)
		{
			SliderMasterVolume->SetValue(hexboundGameInstance->MasterVolume);
			SliderSFXVolume->SetValue(hexboundGameInstance->SFXVolume);
			SliderMusicVolume->SetValue(hexboundGameInstance->MusicVolume);
		}

		if (SliderMasterVolume)
		{
			SliderMasterVolume->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnMasterVolumeChanged);
		}
		if (SliderSFXVolume)
		{
			SliderSFXVolume->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnSFXVolumeChanged);
		}
		if (SliderMusicVolume)
		{
			SliderMusicVolume->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnMusicVolumeChanged);
		}

		if (ButtonBack)
		{
			ButtonBack->OnClicked.AddDynamic(this, &UAudioSettingWidget::OnClickedBack);
		}

		isBinding = true;
	}
}

void UAudioSettingWidget::OnMasterVolumeChanged(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Master Volume OnChanged!! Value : %f"), Value);

	if (hexboundGameInstance)
	{
		hexboundGameInstance->SetMasterVolume(Value);
	}
}

void UAudioSettingWidget::OnSFXVolumeChanged(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("SFX Volume OnChanged!!Value : % f"), Value);

	if (hexboundGameInstance)
	{
		hexboundGameInstance->SetSFXVolume(Value);
	}
}

void UAudioSettingWidget::OnMusicVolumeChanged(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Music Volume OnChanged!! Value : %f"), Value);

	if (hexboundGameInstance)
	{
		hexboundGameInstance->SetMusicVolume(Value);
	}
}

void UAudioSettingWidget::OnClickedBack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Back Button")));

	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::Setting);
	}
}
