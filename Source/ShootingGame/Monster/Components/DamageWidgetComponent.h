#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageWidgetComponent.generated.h"

class UDamageIndicator;

UCLASS()
class SHOOTINGGAME_API UDamageWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

    void BeginPlay() override;

    void InitializePool(UWorld* World, TSubclassOf<UDamageIndicator> WidgetClass, int32 PoolSize);

    UDamageIndicator* GetWidgetFromPool();
    void ReturnWidgetToPool(UDamageIndicator* Widgets);

    UPROPERTY()
    TSubclassOf<UDamageIndicator> DamageWidgetClass;

private:
    TArray<UDamageIndicator*> Pool;

    UWorld* WorldContexts;
};
