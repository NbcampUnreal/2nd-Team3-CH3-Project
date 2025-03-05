#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetDamageIndicator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadeOutFinished);

UCLASS()
class SHOOTINGGAME_API UWidgetDamageIndicator : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetDamageText(float Damage);

	UFUNCTION(BlueprintCallable)
	void PlayFadeOutAnimation();

	FOnFadeOutFinished OnFadeOutFinished;

	UFUNCTION(BlueprintCallable)
	void OnAnimationFinish();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOutAnimation;

private:
};
