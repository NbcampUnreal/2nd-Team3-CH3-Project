#include "Monster/DamageIndicator/DamageActor.h"
#include "Components/WidgetComponent.h"
#include "Monster/DamageIndicator/DamageIndicator.h"

ADamageActor::ADamageActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComponent"));
	WidgetComponent->SetupAttachment(Scene);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);

}

void ADamageActor::BeginPlay()
{
	Super::BeginPlay();	
}



