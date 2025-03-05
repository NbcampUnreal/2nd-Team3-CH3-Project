#include "Item/Item.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = false;
	// 루트 컴포넌트 생성
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	// 스태틱 메시 생성 및 부착
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 위젯 컴포넌트 생성 및 부착
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	OverheadWidget->SetVisibility(false);

	// 충돌 컴포넌트 생성 및 부착
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetSphereRadius(150.0f); //  충돌 범위 조정
	Collision->SetCollisionProfileName(TEXT("Trigger")); // 충돌 프로필 설정
	
	// 충돌 이벤트 바인딩
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
	
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		/*UE_LOG(LogTemp, Warning, TEXT("Overlap Begin: %s"), *OtherActor->GetName());*/
		OverheadWidget->SetVisibility(true);
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		/*UE_LOG(LogTemp, Warning, TEXT("Overlap End: %s"), *OtherActor->GetName());*/
		OverheadWidget->SetVisibility(false);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UWidgetComponent* AItem::GetHud()
{
	return OverheadWidget;
}

