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

	UFUNCTION(BlueprintCallable)
	void UpdateAmmo(int32 CurrentAmmo, int32 MaxAmmo);

protected:
	virtual void NativeConstruct() override;

	// 체력 표시 텍스트 바인딩
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;

	// 체력 바 바인딩
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	// 탄약 표시 텍스트 바인딩
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

};
