// Copyright Epic Games, Inc. All Rights Reserved.


#include "Weapon/Parts.h"

// Sets default values
AParts::AParts()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParts::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParts::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

