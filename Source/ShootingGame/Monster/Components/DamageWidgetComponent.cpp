#include "Monster/Components/DamageWidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Monster/DamageIndicator/DamageIndicator.h"


void UDamageWidgetComponent::BeginPlay()
{
    Super::BeginPlay();

}

void UDamageWidgetComponent::InitializePool(UWorld* World, TSubclassOf<UDamageIndicator> WidgetClasss, int32 PoolSize)
{
    WorldContexts = World;
    this->DamageWidgetClass = WidgetClasss;

    for (int32 i = 0; i < PoolSize; ++i)
    {
        UDamageIndicator* NewWidget = CreateWidget<UDamageIndicator>(WorldContexts, WidgetClasss);
        if (NewWidget)
        {
            NewWidget->AddToViewport();
            NewWidget->SetVisibility(ESlateVisibility::Hidden);
            Pool.Add(NewWidget);
        }
    }
}

UDamageIndicator* UDamageWidgetComponent::GetWidgetFromPool()
{
    // Pool 에 있을시
    if (Pool.Num() > 0)
    {
        UDamageIndicator* NewWidget = Pool.Pop();
        NewWidget->SetVisibility(ESlateVisibility::Visible);
        return NewWidget;
    }

    // Pool에 없으면 새로 추가
    UDamageIndicator* NewWidget = CreateWidget<UDamageIndicator>(WorldContexts, this->DamageWidgetClass);
    if (NewWidget)
    {
        NewWidget->AddToViewport();
    }

    return NewWidget;
}

void UDamageWidgetComponent::ReturnWidgetToPool(UDamageIndicator* Widgets)
{
    if (Widgets)
    {
        Widgets->SetVisibility(ESlateVisibility::Hidden);
        Pool.Add(Widgets);
    }
}
