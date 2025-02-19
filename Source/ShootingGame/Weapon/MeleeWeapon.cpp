// Copyright Epic Games, Inc. All Rights Reserved.

#include "Weapon/MeleeWeapon.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AMeleeWeapon::AMeleeWeapon()
{

}

void AMeleeWeapon::Attack()
{
	Super::Attack();
	USkeletalMeshComponent* SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();
	SkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnEnemyOverlap);
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