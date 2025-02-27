#include "UI/VideoSettingWidget.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"

void UVideoSettingWidget::NativeConstruct()
{
	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();

		if (ButtonBack)
		{
			ButtonBack->OnClicked.AddDynamic(this, &UVideoSettingWidget::OnClickedBack);
		}

		isBinding = true;
	}
}

void UVideoSettingWidget::OnClickedBack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Back Button")));

	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::Setting);
	}
}
