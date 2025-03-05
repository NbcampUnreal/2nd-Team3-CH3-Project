#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DamageWidgetPool.generated.h"

class UDamageIndicator;

UCLASS()
class SHOOTINGGAME_API UDamageWidgetPool : public UObject
{
	GENERATED_BODY()

public:
    void InitializePool(UWorld* World, TSubclassOf<UDamageIndicator> WidgetClass, int32 PoolSize);

    UDamageIndicator* GetWidgetFromPool();
    void ReturnWidgetToPool(UDamageIndicator* Widget);

private:
    TArray<UDamageIndicator*> Pool;

    UPROPERTY()
    TSubclassOf<UDamageIndicator> TextWidgetClass;

    UWorld* WorldContext;
};
