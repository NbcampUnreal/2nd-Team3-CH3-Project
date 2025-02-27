#include "UI/PauseWidget.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"

#include "Kismet/GameplayStatics.h"
#include "Default/DefaultCharacter.h"
#include "Character/MyCharacter.h"
#include "Core/HexboundGameInstance.h"

void UPauseWidget::NativeConstruct()
{
	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();

		if (ButtonResume)
		{
			ButtonResume->OnClicked.AddDynamic(this, &UPauseWidget::OnClickedResume);
		}

		if (ButtonHome)
		{
			ButtonHome->OnClicked.AddDynamic(this, &UPauseWidget::OnClickedHome);
		}

		isBinding = true;
	}
}

void UPauseWidget::OnClickedSetting()
{
	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::Setting);
	}
}

void UPauseWidget::OnClickedResume()
{
	if (uiManager)
	{
		ACharacter* playedCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (ADefaultCharacter* defaultCharacter = Cast<ADefaultCharacter>(playedCharacter))
		{
			defaultCharacter->OnInputESCKey();
		}
		else if (AMyCharacter* myCharacter = Cast<AMyCharacter>(playedCharacter))
		{
			myCharacter->OnInputESCKey();
		}
	}
}

void UPauseWidget::OnClickedHome()
{
	UGameInstance* gameInstance = GetGameInstance();

	if (UHexboundGameInstance* hexboundGameInstance = Cast<UHexboundGameInstance>(gameInstance))
	{
		hexboundGameInstance->OpenLevel(ELevel::MenuLevel);
	}

	// TO DO : 게임 종료 관련 로직 추가

}
