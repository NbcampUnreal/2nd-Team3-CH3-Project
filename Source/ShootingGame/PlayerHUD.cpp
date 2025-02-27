// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (CrosshairWidgetClass)
	{
		CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport();
		}
	}
	if (CrosshairWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("CrosshairWidget"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CrosshairWidget fail"));
	}
}

void APlayerHUD::SetCrosshairSize(bool bZoomedIn)
{
	if (CrosshairWidget)
	{
		UImage* CrosshairImage = Cast<UImage>(CrosshairWidget->GetWidgetFromName(TEXT("CrosshairImage")));
		if (CrosshairImage)
		{
			FVector2D NewScale = bZoomedIn ? FVector2D(0.5f, 0.5f) : FVector2D(1.0f, 1.0f);
			CrosshairImage->SetRenderScale(NewScale);
		}
	}
}