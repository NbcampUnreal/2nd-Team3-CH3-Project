#include "Monster/Components/DamageTextComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

UDamageTextComponent::UDamageTextComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

    InitializePool();
}


void UDamageTextComponent::BeginPlay()
{
	Super::BeginPlay();


    //// Ǯ �ʱ�ȭ (��: 10���� ���� �̸� ����)
    //for (int32 i = 0; i < 10; ++i)
    //{
    //    UUserWidget* Widget = CreateWidget(GetWorld(), DamageTextWidgetClass);
    //    if (Widget)
    //    {
    //        Widget->AddToViewport();
    //        Widget->SetVisibility(ESlateVisibility::Hidden);
    //        WidgetPool.Add(Widget);
    //    }
    //}
}

void UDamageTextComponent::InitializePool()
{
    if (DamageTextWidgetClass)
    {
        for (int32 i = 0; i < PoolSize; ++i)
        {
            UWidgetComponent* NewWidgetComponent = NewObject<UWidgetComponent>(this);
            NewWidgetComponent->SetWidgetClass(DamageTextWidgetClass);
            NewWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
            NewWidgetComponent->SetDrawSize(FVector2D(200, 100));
            NewWidgetComponent->SetVisibility(false);
            NewWidgetComponent->RegisterComponent(); // ������Ʈ ���
            WidgetPool.Add(NewWidgetComponent);
        }
    }
}

UWidgetComponent* UDamageTextComponent::GetAvailableWidgetFromPool()
{
    for (UWidgetComponent* WidgetComponent : WidgetPool)
    {
        if (!WidgetComponent->IsVisible())
        {
            return WidgetComponent;
        }
    }
    return nullptr;
}

void UDamageTextComponent::ShowDamageText(float DamageAmount)
{
    UWidgetComponent* DamageTextWidgetComponent = GetAvailableWidgetFromPool();
    if (DamageTextWidgetComponent && DamageTextWidgetClass)
    {
        // ���� ��ġ ���� (������ ������ �Ӹ� ��)
        FVector WidgetLocation = GetOwner()->GetActorLocation() + FVector(0, 0, 200); // Z ������ ������ ����
        DamageTextWidgetComponent->SetWorldLocation(WidgetLocation);

        // ���� ǥ��
        DamageTextWidgetComponent->SetVisibility(true);

        // ������ �ؽ�Ʈ ����
        UUserWidget* DamageTextWidget = DamageTextWidgetComponent->GetWidget();
        if (DamageTextWidget)
        {
            // DamageTextWidget�� ������ ���� �����ϴ� ���� �߰�
            // ��: DamageTextWidget->SetDamageText(DamageAmount);
        }

        // ���� �ð� �� ���� �����
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, DamageTextWidgetComponent]()
            {
                ReturnWidgetToPool(DamageTextWidgetComponent);
            }, 2.0f, false); // 2�� �� ����
    }

}

void UDamageTextComponent::ReturnWidgetToPool(UWidgetComponent* WidgetComponent)
{
    if (WidgetComponent)
    {
        WidgetComponent->SetVisibility(false);
    }
}


