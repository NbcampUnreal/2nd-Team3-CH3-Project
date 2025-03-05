#include "Monster/DamageIndicator/DamageWidgetPool.h"
#include "Blueprint/UserWidget.h"
#include "Monster/DamageIndicator/DamageIndicator.h"

void UDamageWidgetPool::InitializePool(UWorld* World, TSubclassOf<UDamageIndicator> WidgetClass, int32 PoolSize)
{
    WorldContext = World;
    TextWidgetClass = WidgetClass;

    for (int32 i = 0; i < PoolSize; ++i)
    {
        UDamageIndicator* Widget = CreateWidget<UDamageIndicator>(WorldContext, WidgetClass);
        if (Widget)
        {
            Widget->AddToViewport();
            Widget->SetVisibility(ESlateVisibility::Hidden);
            Pool.Add(Widget);
        }
    }
}


UDamageIndicator* UDamageWidgetPool::GetWidgetFromPool()
{
    // Pool 에 있을시
    if (Pool.Num() > 0)
    {
        UDamageIndicator* Widget = Pool.Pop();
        Widget->SetVisibility(ESlateVisibility::Visible);
        return Widget;
    }

    // Pool에 없으면 새로 추가
    UDamageIndicator* NewWidget = CreateWidget<UDamageIndicator>(WorldContext, TextWidgetClass);
    if (NewWidget)
    {
        NewWidget->AddToViewport();
    }

    return NewWidget;
}

void UDamageWidgetPool::ReturnWidgetToPool(UDamageIndicator* Widget)
{
    if (Widget)
    {
        Widget->SetVisibility(ESlateVisibility::Hidden);
        Pool.Add(Widget);
    }
}