#include "UI/AchievementWidget.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"

void UAchievementWidget::NativeConstruct()
{
	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();

		if (ButtonBack)
		{
			ButtonBack->OnClicked.AddDynamic(this, &UAchievementWidget::OnClickedBack);
		}

		isBinding = true;
	}
}

void UAchievementWidget::OnClickedBack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Back Button")));

	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::MainMenu);
	}
}
