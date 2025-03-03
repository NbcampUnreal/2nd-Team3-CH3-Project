// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/InGame.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

void UInGame::NativeConstruct()
{
	Super::NativeConstruct();
	if (loadedAmmoText)
	{
		loadedAmmoText->SetText(FText::FromString(FString::Printf(TEXT("0"))));
	}

	if (remainingAmmoText)
	{
		remainingAmmoText->SetText(FText::FromString(FString::Printf(TEXT("0"))));
	}
}


void UInGame::UpdateAmmoWidget(int32 LoadedAmmo, int32 RemainingAmmo)
{
	if (!loadedAmmoText || !remainingAmmoText)
	{
		UE_LOG(LogTemp, Error, TEXT("TextBlock is NULL! UI update failed."));
		return;
	}

	if (loadedAmmoText)
	{
		loadedAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), LoadedAmmo)));
	}

	if (remainingAmmoText)
	{
		remainingAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), RemainingAmmo)));
	}
}