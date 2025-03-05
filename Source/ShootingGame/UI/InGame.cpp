// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/InGame.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"

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

	if (currentWeapon)
	{
		currentWeapon->SetText(FText::FromString(FString::Printf(TEXT("Fist"))));
	}

	WeaponImageTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/Weapons/Images/T_FistImage.T_FistImage"));

	if (WeaponUIImage)
	{
		WeaponUIImage->SetBrushFromTexture(WeaponImageTexture);
	}
}


void UInGame::UpdateAmmo(int32 LoadedAmmo, int32 RemainingAmmo)
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

void UInGame::PrintCurrentWeapon(FName WeaponType, UTexture2D* WeaponImage)
{
	if (currentWeapon)
	{
		currentWeapon->SetText(FText::FromString(WeaponType.ToString()));
	}

	if (WeaponUIImage)
	{
		WeaponUIImage->SetBrushFromTexture(WeaponImage);
	}
}