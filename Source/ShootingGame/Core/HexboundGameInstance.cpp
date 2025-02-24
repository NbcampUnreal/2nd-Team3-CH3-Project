#include "HexboundGameInstance.h"
#include "HexboundPlayerController.h"
#include "Managers/UIManager.h"

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


void UHexboundGameInstance::InitUIMangerProperties()
{
	UUIManager* UIManager = GetSubsystem<UUIManager>();
	if (UIManager)
	{
		UIManager->SetWidgetClasses(WidgetClasses);
		UE_LOG(LogTemp, Warning, TEXT("Set Widget Classes"));
	}

}

