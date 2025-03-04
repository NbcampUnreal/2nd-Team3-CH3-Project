// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HexPlayerHUD.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"

void UHexPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHexPlayerHUD::UpdateHealth(float CurrentHealth, float MaxHealth)
{
    if (HealthText && HealthBar)
    {
        FString HealthString = FString::Printf(TEXT("%d / %d"),
            FMath::RoundToInt(CurrentHealth), FMath::RoundToInt(MaxHealth));
        HealthText->SetText(FText::FromString(HealthString));

        float Ratio = CurrentHealth / MaxHealth;
        HealthBar->SetPercent(Ratio);
    }
}

void UHexPlayerHUD::UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo)
{
    if (AmmoText)
    {
        FString AmmoString = FString::Printf(TEXT("%d / %d"), CurrentAmmo, MaxAmmo);
        AmmoText->SetText(FText::FromString(AmmoString));
    }
}