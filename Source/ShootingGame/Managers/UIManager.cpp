#include "UIManager.h"
#include "Core/HexboundPlayerController.h"
#include "Core/HexboundGameState.h"
#include "Kismet/GameplayStatics.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

UUIManager::UUIManager()
{

}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Init UIManager"));

	World = GetGameInstance()->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ShowMainUI: World is null"));
		return;
	}

}

void UUIManager::ShowUI(EHUDState state)
{
	if (CurrentWidgetInstance)
	{
		CurrentWidgetInstance->RemoveFromParent();
		CurrentWidgetInstance = nullptr;
	}

	if (WidgetInstances.Num() != 0)
	{
		WidgetInstances.Empty();
	}

	//int index = static_cast<int>(state);
	TSubclassOf<UUserWidget> WidgetClass = WidgetClasses[static_cast<int>(state)];

	if (WidgetClass)
	{
		UUserWidget* widgetInstance = CreateWidget<UUserWidget>(World, WidgetClass);

		CurrentWidgetInstance = widgetInstance;
		WidgetInstances.Add(widgetInstance);

		if (widgetInstance)
		{
			widgetInstance->AddToViewport();
		}

		if (HexboundController) HexboundController->ShowCursor(true);
	}

}
