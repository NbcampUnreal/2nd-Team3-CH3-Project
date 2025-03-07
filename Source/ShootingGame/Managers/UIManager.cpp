#include "UIManager.h"
#include "Core/HexboundPlayerController.h"
#include "Core/HexboundGameState.h"
#include "Core/HexboundGameInstance.h"
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

	//World = GetGameInstance()->GetWorld();
	if (!GetGameInstance()->GetWorld())
	{
		UE_LOG(LogTemp, Error, TEXT("ShowMainUI: World is null"));
		return;
	}

}

TSubclassOf<UUserWidget> UUIManager::GetWidgetClasse(EHUDState state)
{
	return WidgetClasses[static_cast<int>(state)];
}

void UUIManager::SetWidgetClasses(TArray<TSubclassOf<UUserWidget>> widgets)
{
	UWorld* World = GetGameInstance()->GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("ShowMainUI: World is null"));
		return;
	}

	WidgetClasses = widgets;
	for (int i = 0; i < widgets.Num(); i++)
	{
		EHUDState state = static_cast<EHUDState>(i);

		UUserWidget* widgetInstance = CreateWidget<UUserWidget>(World, WidgetClasses[i]);
		WidgetInstances.Add(state, widgetInstance);
	}
}


void UUIManager::SetUIState(EHUDState state)
{
	switch (state)
	{
	case EHUDState::MenuBase:
		break;
	case EHUDState::MainMenu:
		HideAllUI();
		ShowUI(EHUDState::MenuBase);
		ShowUI(EHUDState::MainMenu);
		break;
	case EHUDState::Setting:
		HideAllUI();
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			UHexboundGameInstance* HexboundGameInstance = Cast<UHexboundGameInstance>(GameInstance);
			if (HexboundGameInstance)
			{
				if(HexboundGameInstance->GetCurrentLevel() == ELevel::MenuLevel) ShowUI(EHUDState::MenuBase);
			}
		}
		ShowUI(EHUDState::Setting);
		break;
	case EHUDState::AudioSetting:
		HideUI(EHUDState::Setting);
		ShowUI(EHUDState::AudioSetting);
		break;
	case EHUDState::VideoSetting:
		HideUI(EHUDState::Setting);
		ShowUI(EHUDState::VideoSetting);
		break;
	case EHUDState::KeyMapping:
		HideUI(EHUDState::Setting);
		ShowUI(EHUDState::KeyMapping);
		break;
	case EHUDState::Archivements:
		HideUI(EHUDState::MainMenu);
		ShowUI(EHUDState::Archivements);
		break;
	case EHUDState::InGameBase:
		HideAllUI();
		ShowUI(EHUDState::InGameBase);
		break;
	case EHUDState::GameOver:
		HideAllUI();
		ShowUI(EHUDState::GameOver);
	default:
		break;
	}

}

void UUIManager::ShowUI(EHUDState state)
{
	if (WidgetInstances[state])
	{
		WidgetInstances[state]->AddToViewport();
	}
}

void UUIManager::HideUI(EHUDState state)
{
	if (WidgetInstances[state])
	{
		WidgetInstances[state]->RemoveFromParent();
	}
}

void UUIManager::HideAllUI()
{
	for (auto WidgetInstance : WidgetInstances)
	{
		WidgetInstance.Value->RemoveFromParent();
	}
}


void UUIManager::ShowInventory(bool isShow)
{
	UE_LOG(LogTemp, Warning, TEXT("Show Inventory"));
}
