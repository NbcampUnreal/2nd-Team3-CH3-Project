// Copyright Epic Games, Inc. All Rights Reserved.

#include "Weapon/ThrowableWeapon.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Weapon/ThrowableProjectile.h"
#include "Kismet/GameplayStatics.h"

AThrowableWeapon::AThrowableWeapon()
{
	LocationCheck = CreateDefaultSubobject<USphereComponent>(TEXT("LocationCheck"));
	LocationCheck->SetupAttachment(RootComponent);
	LocationCheck->InitSphereRadius(10.0f);

	WeaponMesh->SetSkeletalMesh(nullptr);

	ThrowableProjectile = nullptr;
	ThrowableProjectileClass = nullptr;
	ThrowForce = 1200.0f;
	CurrentPossession = 0;
	MaxPossession = 10;
	bIsInHand = false;
}

int32 AThrowableWeapon::GetCurrentPossession() const
{
	return CurrentPossession;
}

int32 AThrowableWeapon::GetMaxPossession() const
{
	return MaxPossession;
}

void AThrowableWeapon::AddPossession(int32 AddNumber)
{
	CurrentPossession = FMath::Min(CurrentPossession + AddNumber, MaxPossession);
}

void AThrowableWeapon::Spawn()
{
	if (!bIsInHand && CurrentPossession > 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ThrowableProjectile = GetWorld()->SpawnActor<AThrowableProjectile>(ThrowableProjectileClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
		if (ThrowableProjectile)
		{
			if (ThrowableProjectile->AttachToComponent(this->RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale))
			{
				ThrowableProjectile->SpawnSetting();
				CurrentPossession--;
				bIsInHand = true;
			}
		}
	}
}

void AThrowableWeapon::ReadyToExplosion()
{
	if (ThrowableProjectile)
	{
		ThrowableProjectile->ExplosionCount();
	}
}

void AThrowableWeapon::Throw()
{
	if (ThrowableProjectile)
	{
		ThrowableProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ThrowableProjectile->Thrown(ThrowForce);
		bIsInHand = false;
	}
}
