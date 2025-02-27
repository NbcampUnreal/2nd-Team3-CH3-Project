#include "UI/AudioSettingWidget.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"

void UAudioSettingWidget::NativeConstruct()
{
	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();

		if (ButtonBack)
		{
			ButtonBack->OnClicked.AddDynamic(this, &UAudioSettingWidget::OnClickedBack);
		}

		isBinding = true;
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
