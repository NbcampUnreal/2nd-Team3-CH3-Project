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


    //// 풀 초기화 (예: 10개의 위젯 미리 생성)
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
            NewWidgetComponent->RegisterComponent(); // 컴포넌트 등록
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
        // 위젯 위치 설정 (소유자 액터의 머리 위)
        FVector WidgetLocation = GetOwner()->GetActorLocation() + FVector(0, 0, 200); // Z 축으로 오프셋 적용
        DamageTextWidgetComponent->SetWorldLocation(WidgetLocation);

        // 위젯 표시
        DamageTextWidgetComponent->SetVisibility(true);

        // 데미지 텍스트 설정
        UUserWidget* DamageTextWidget = DamageTextWidgetComponent->GetWidget();
        if (DamageTextWidget)
        {
            // DamageTextWidget에 데미지 값을 전달하는 로직 추가
            // 예: DamageTextWidget->SetDamageText(DamageAmount);
        }

        // 일정 시간 후 위젯 숨기기
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, DamageTextWidgetComponent]()
            {
                ReturnWidgetToPool(DamageTextWidgetComponent);
            }, 2.0f, false); // 2초 후 숨김
    }

}

void UDamageTextComponent::ReturnWidgetToPool(UWidgetComponent* WidgetComponent)
{
    if (WidgetComponent)
    {
        WidgetComponent->SetVisibility(false);
    }
}


