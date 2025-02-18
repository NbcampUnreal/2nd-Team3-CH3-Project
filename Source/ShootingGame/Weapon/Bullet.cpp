// Copyright Epic Games, Inc. All Rights Reserved.

#include "Weapon/Bullet.h"
#include "..\..\Source\ShootingGame\Weapon\Firearm.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	SetRootComponent(bulletMesh);

	bulletMesh->SetCollisionProfileName("BlockAll");
	bulletMesh->BodyInstance.bUseCCD = true;

	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	projectileMovement->InitialSpeed = 3000.0f;
	projectileMovement->MaxSpeed = 3000.0f;
	projectileMovement->bRotationFollowsVelocity = true;
	projectileMovement->bShouldBounce = false;
	projectileMovement->ProjectileGravityScale = 1.0f;

	bulletMesh->OnComponentHit.AddDynamic(this, &ABullet::onHit);
}

void ABullet::ActivateBullet(FVector SpawnLocation, FRotator SpawnRotation, float Speed)
{
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	if (projectileMovement)
	{
		projectileMovement->InitialSpeed = Speed;
		projectileMovement->MaxSpeed = Speed;
	}
}

void ABullet::SetFirearm(AFirearm* Firearm)
{
	if (Firearm)
	{
		gun = Firearm;
	}
}

void ABullet::SetBulletMesh(UStaticMesh* BulletMesh)
{
	if (BulletMesh)
	{
		bulletMesh->SetStaticMesh(BulletMesh);
	}
}

void ABullet::SetBulletSpeed(float Speed)
{
	projectileMovement->InitialSpeed = Speed;
	projectileMovement->MaxSpeed = Speed;
}

void ABullet::onHit(
	UPrimitiveComponent* HitComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (gun)
	{
		if (OtherActor && OtherActor != this)
		{
			gun->DealDamage(OtherActor);

			if (hitEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			if (hitDecal)
			{
				UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f), Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

			if (hitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), hitSound, Hit.ImpactPoint);
			}
		}

		gun->ReturnBulletToPool(this);
	}
}