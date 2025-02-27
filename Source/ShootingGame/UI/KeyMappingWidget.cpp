// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/KeyMappingWidget.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"

void UKeyMappingWidget::NativeConstruct()
{
	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();

		if (ButtonBack)
		{
			ButtonBack->OnClicked.AddDynamic(this, &UKeyMappingWidget::OnClickedBack);
		}

		isBinding = true;
	}
}

void UKeyMappingWidget::OnClickedBack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Back Button")));

	if (uiManager)
	{
		uiManager->SetUIState(EHUDState::Setting);
	}
}
