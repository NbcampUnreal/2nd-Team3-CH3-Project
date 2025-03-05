#include "Monster/DamageIndicator/DamageIndicator.h"
#include "Components/WidgetComponent.h"
#include "Monster/DamageIndicator/WidgetDamageIndicator.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"

ADamageIndicator::ADamageIndicator()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	WidgetComponent->SetupAttachment(Scene);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
}

void ADamageIndicator::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ADamageIndicator::DestroySelf, 1.0f, false);

	//if (DamageWidgetClass)
	//{
	//	WidgetComponent->SetWidgetClass(DamageWidgetClass);
	//	WidgetComponent->InitWidget();
	//}
}

void ADamageIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCameraManager)
	{
		FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
		FVector IndicatorLocation = GetActorLocation();

		// 카메라 위치를 바라보는 회전 계산
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(CameraLocation - IndicatorLocation).Rotator();
		SetActorRotation(LookAtRotation);
	}
}

void ADamageIndicator::SetDamage(float Damage)
{
	UWidgetDamageIndicator* DamageWidget = Cast<UWidgetDamageIndicator>(WidgetComponent->GetUserWidgetObject());
	if (DamageWidget)
	{
		// 데미지 값 설정
		DamageWidget->SetDamageText(Damage);
	}
}

void ADamageIndicator::DestroySelf()
{
	Destroy();
}

//void ADamageIndicator::OnFadeOutFinished()
//{
//	// 애니메이션이 끝나면 액터를 제거
//	Destroy();
//}

