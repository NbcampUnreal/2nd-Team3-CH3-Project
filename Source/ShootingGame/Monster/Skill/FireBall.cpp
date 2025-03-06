#include "Monster/Skill/FireBall.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

AFireBall::AFireBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AFireBall::OnFireBallOverlap);
}

void AFireBall::InitializeFireBall(float InSpeed, float InDamage, FVector InDirection)
{
	Speed = InSpeed;
	Damage = InDamage;
	MoveDirection = InDirection.GetSafeNormal(); // 정규화하여 방향 벡터로 설정
}

void AFireBall::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3.0f);
}


void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Movement = MoveDirection * Speed * DeltaTime;

	AddActorWorldOffset(Movement, true);
}

void AFireBall::OnFireBallOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("FireBall Overlap!!")));
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());
		Destroy();
	}
}


