#include "UI/GameOverWidget.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"
#include "Core/HexboundGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();

		// ��ư�� nullptr�� �ƴ� ���� ���ε�
		if (ButtonHome)
		{
			ButtonHome->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickHomeButton);
		}
		if (ButtonRestart)
		{
			ButtonRestart->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickRestartButton);
		}
		if (ButtonExit)
		{
			ButtonExit->OnClicked.AddDynamic(this, &UGameOverWidget::OnClickExitButton);
		}

		isBinding = true;
	}
}

void UGameOverWidget::OnClickHomeButton()
{
	if (uiManager)
	{
		uiManager->HideAllUI();
		if (UGameInstance* gameInstance = GetGameInstance())
		{
			UHexboundGameInstance* hexboundGameInstance = Cast<UHexboundGameInstance>(gameInstance);
			if (hexboundGameInstance) hexboundGameInstance->OpenLevel(ELevel::MenuLevel);
		}

		uiManager->SetUIState(EHUDState::MainMenu);

	}
}

void UGameOverWidget::OnClickRestartButton()
{
	if (uiManager)
	{
		uiManager->HideAllUI();
		if (UGameInstance* gameInstance = GetGameInstance())
		{
			UHexboundGameInstance* hexboundGameInstance = Cast<UHexboundGameInstance>(gameInstance);
			if (hexboundGameInstance) hexboundGameInstance->OpenLevel(ELevel::Main);
		}

		uiManager->SetUIState(EHUDState::InGameBase);
	}
}

void UGameOverWidget::OnClickExitButton()
{
	UKismetSystemLibrary::QuitGame(this, 0, EQuitPreference::Quit, false);
}
