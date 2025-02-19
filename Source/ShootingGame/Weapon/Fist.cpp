// Copyright Epic Games, Inc. All Rights Reserved.

#include "Fist.h"
#include "Components/SphereComponent.h"

AFist::AFist()
{
	FistCoillision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	FistCoillision->SetupAttachment(RootComponent);

	this->Damage = 5.0f;
	this->AttackRate = 1.0f;
}

void AFist::Attack()
{
	Super::Attack();
	FistCoillision->OnComponentBeginOverlap.AddDynamic(this, &AFist::OnEnemyOverlap);
}