#include "Monster/DamageIndicator/DamageIndicator.h"
#include "Components/TextBlock.h"


void UDamageIndicator::SetDamageText(float Damage)
{
    if (DamageText)
    {
        DamageText->SetText(FText::AsNumber(FMath::RoundToInt(Damage)));
    }

    if (FadeOutAnimation)
    {
        PlayAnimation(FadeOutAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
    }

}

void UDamageIndicator::ReturnToPool()
{

}
