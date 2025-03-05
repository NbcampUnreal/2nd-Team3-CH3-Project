#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageIndicator.generated.h"

class UTextBlock;

UCLASS()
class SHOOTINGGAME_API UDamageIndicator : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDamageText(float Damage);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DamageText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOutAnimation;

	void ReturnToPool();
	
};
