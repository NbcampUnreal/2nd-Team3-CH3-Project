#include "Monster/DamageIndicator/WidgetDamageIndicator.h"
#include "Components/TextBlock.h"

void UWidgetDamageIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	PlayFadeOutAnimation();
}

void UWidgetDamageIndicator::SetDamageText(float Damage)
{
    if (DamageText)
    {
        DamageText->SetText(FText::AsNumber(FMath::RoundToInt(Damage)));
    } 
}

void UWidgetDamageIndicator::PlayFadeOutAnimation()
{
	if (FadeOutAnimation)
	{
		PlayAnimation(FadeOutAnimation);
	}
}

void UWidgetDamageIndicator::OnAnimationFinish()
{
	OnFadeOutFinished.Broadcast();
}

