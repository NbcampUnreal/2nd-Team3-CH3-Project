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

    // �Ҵ��� UserWidget Ŭ����
    UPROPERTY(EditDefaultsOnly, Category = "Widget")
    TSubclassOf<UUserWidget> DamageTextWidgetClass;

    // Ǯ���� ������ �������� �Լ�
   // UUserWidget* GetWidgetFromPool();

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    int32 PoolSize = 5;

    // ���� Ǯ �ʱ�ȭ
    void InitializePool();

    // ��� ������ ������ Ǯ���� ��������
    UWidgetComponent* GetAvailableWidgetFromPool();

    // Ǯ�� ������ ��ȯ�ϴ� �Լ�
    void ReturnWidgetToPool(UWidgetComponent* WidgetComponent);
};
