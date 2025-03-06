// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HexPlayerHUD.generated.h"

UCLASS()
class SHOOTINGGAME_API UHexPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateHealth(float CurrentHealth, float MaxHealth);

protected:
	virtual void NativeConstruct() override;

	//HP bar
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

};
