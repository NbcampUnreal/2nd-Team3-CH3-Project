#include "HexboundGameInstance.h"
#include "HexboundPlayerController.h"
#include "Managers/UIManager.h"
#include "Kismet/GameplayStatics.h"

#include "Blueprint/UserWidget.h"

UHexboundGameInstance::UHexboundGameInstance()
{
	currentLevel = ELevel::MenuLevel;
}

void UHexboundGameInstance::Init()
{
	Super::Init();
	InitUIMangerProperties();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{

		InitUIMangerProperties();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Set HexboundPlayerController At UIMangaer"));
	}

}

ELevel UHexboundGameInstance::GetCurrentLevel()
{
	return currentLevel;
}

void UHexboundGameInstance::SetCurrentLevel(ELevel level)
{
	currentLevel = level;
}

void UHexboundGameInstance::OpenLevel(ELevel level)
{
	SetCurrentLevel(level);

	FName levelName = FName(*StaticEnum<ELevel>()->GetNameStringByValue(static_cast<int64>(level)));
	UGameplayStatics::OpenLevel(GetWorld(), levelName);
}


void UHexboundGameInstance::SetMasterVolume(float Volume)
{
	MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), nullptr, nullptr, MasterVolume, 1.0f, 0.0f, true);
}

void UHexboundGameInstance::SetSFXVolume(float Volume)
{
	SFXVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

void UHexboundGameInstance::SetMusicVolume(float Volume)
{
	MusicVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}


void UHexboundGameInstance::InitUIMangerProperties()
{
	UUIManager* UIManager = GetSubsystem<UUIManager>();
	if (UIManager)
	{
		UIManager->SetWidgetClasses(WidgetClasses);
		UE_LOG(LogTemp, Warning, TEXT("Set Widget Classes"));
	}

}

