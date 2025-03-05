#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageTextComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGGAME_API UDamageTextComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDamageTextComponent();

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void ShowDamageText(float DamageAmount);
		
private:
    // Object Pool
    UPROPERTY()
    TArray<class UWidgetComponent*> WidgetPool;

    // 할당할 UserWidget 클래스
    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    TSubclassOf<UUserWidget> DamageTextWidgetClass;

    // 풀에서 위젯을 가져오는 함수
   // UUserWidget* GetWidgetFromPool();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    int32 PoolSize = 5;

    // 위젯 풀 초기화
    void InitializePool();

    // 사용 가능한 위젯을 풀에서 가져오기
    UWidgetComponent* GetAvailableWidgetFromPool();

    // 풀에 위젯을 반환하는 함수
    void ReturnWidgetToPool(UWidgetComponent* WidgetComponent);
};
