// Copyright Epic Games, Inc. All Rights Reserved.

#include "Weapon/MeleeWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AMeleeWeapon::AMeleeWeapon()
{
	USkeletalMesh* MeleeWeapon = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Weapons/MeleeWeapon/Knife/SK_M9_Knife_X.SK_M9_Knife_X"));
	if (MeleeWeapon)
	{
		WeaponMesh->SetSkeletalMesh(MeleeWeapon);
	}
}

void AMeleeWeapon::Attack()
{
	Super::Attack();
	if (USkeletalMeshComponent* SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>())
	{
		SkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnEnemyOverlap);
	}
}

void AMeleeWeapon::OnEnemyOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyindex,
	bool bFromSweep,
	const FHitResult& Sweep)
{
	if (OtherActor && OtherActor->ActorHasTag("Enemy"))
	{
		DealDamage(OtherActor);

		if (HitEffect)
		{
			UNiagaraComponent* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(), HitEffect, Sweep.Location, FRotator::ZeroRotator);
		}

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Sweep.Location);
		}
	}
}