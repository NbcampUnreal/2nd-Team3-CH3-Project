#include "HexboundGameState.h"
#include "HexboundGameInstance.h"
#include "HexboundPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Managers/UIManager.h"

AHexboundGameState::AHexboundGameState()
{

}

void AHexboundGameState::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		HexBoundPlayerController = Cast<AHexboundPlayerController>(PlayerController);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Set HexboundPlayerController At UIMangaer"));
	}

	InitUIMangerProperties();
}

void AHexboundGameState::InitUIMangerProperties()
{
	// TO DO : Intro UI 후 메인 메뉴 UI 보이도록 수정 예정
	UUIManager* UIManager = GetGameInstance()->GetSubsystem<UUIManager>();
	if (UIManager)
	{
		UIManager->WidgetClasses = WidgetClasses;
		UE_LOG(LogTemp, Warning, TEXT("Set Widget Classes"));

		UIManager->HexboundController = HexBoundPlayerController;

		UIManager->ShowUI(EHUDState::MainMenu);
	}
}


void AHexboundGameState::OnGameStart()
{
	// TO DO : Level Load ( Main Level )

	HexBoundPlayerController->ShowCursor(false);

}