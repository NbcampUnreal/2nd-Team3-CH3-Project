#include "Monster/BossMonster.h"

ABossMonster::ABossMonster()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABossMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
