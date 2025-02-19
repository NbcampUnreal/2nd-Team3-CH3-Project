// Copyright Epic Games, Inc. All Rights Reserved.

#include "Firearm.h"
#include "Weapon/Bullet.h"
#include "Weapon/Magazine.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AFirearm::AFirearm()
{
	PrimaryActorTick.bCanEverTick = false;

	BulletMesh = nullptr;
	BulletSpeed = 3000.0f;
	MaxAmmo = 200;
	CurrentAmmo = 0;
	MaxReloadedAmmo = 0;
	ReloadedAmmo = 0;
	ReloadTime = 1.0f;
	bIsLoadingComplete = true;
	bIsMagazineAttached = false;
}

void AFirearm::Attack()
{
	if (ReloadedAmmo > 0 && bIsLoadingComplete)
	{
		if (bIsCooltimeEnd)
		{
			if (AttackNiagara)
			{
				UNiagaraComponent* AttackEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
					AttackNiagara,
					WeaponMesh,
					TEXT("MuzzleSocket"),
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					EAttachLocation::SnapToTarget,
					true
				);
			}

			Fire();

			ReloadedAmmo--;
		}
		Super::Attack();
	}

	if (ReloadedAmmo <= 0 && bIsCooltimeEnd)
	{
		if (EmptyAmmoSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptyAmmoSound, GetActorLocation());
		}
	}

}

int32 AFirearm::GetCurrentAmmoValue() const
{
	return CurrentAmmo;
}

void AFirearm::AddAmmo(int32 AmmoToAdd)
{
	CurrentAmmo = FMath::Clamp(CurrentAmmo + AmmoToAdd, 0, MaxAmmo);
}

void AFirearm::Reload()
{
	if (bIsLoadingComplete && bIsMagazineAttached)
	{
		if (ReloadSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReloadSound, GetActorLocation());
		}
		bIsLoadingComplete = false;
		GetWorldTimerManager().SetTimer(
			ReloadTimerHandle,
			[this]() {
				int32 ReloadValue = FMath::Min(MaxReloadedAmmo - ReloadedAmmo, CurrentAmmo);
				CurrentAmmo = FMath::Max(CurrentAmmo - ReloadValue, 0);
				ReloadedAmmo = MaxReloadedAmmo;
				bIsLoadingComplete = true;
			},
			ReloadTime,
			false
		);
	}
}

void AFirearm::EquipParts(AParts* Parts)
{
	if (Parts)
	{
		DetachParts(Parts->SocketNameForAttach);
		Parts->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, Parts->SocketNameForAttach);

		if (Parts->IsA(AMagazine::StaticClass()))
		{
			AMagazine* Magazine = Cast<AMagazine>(Parts);
			MaxReloadedAmmo = Magazine->GetMagazineCapacity();
			ReloadedAmmo = MaxReloadedAmmo;
			bIsMagazineAttached = true;
		}
	}
}

void AFirearm::Fire()
{
	if (BulletClass)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();

		ABullet* Bullet = nullptr;

		if (bulletPool.Num() > 0)
		{
			Bullet = bulletPool.Pop();
			if (Bullet)
			{
				Bullet->ActivateBullet(SpawnLocation, SpawnRotation, BulletSpeed);
			}
		}

		else
		{
			if (BulletClass)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;

				Bullet->SetBulletSpeed(BulletSpeed);
				Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, SpawnRotation, SpawnParams);
			}
		}

		if (Bullet)
		{
			Bullet->SetFirearm(this);	
			Bullet->SetBulletMesh(BulletMesh);	
		}
	}
}

void AFirearm::ReturnBulletToPool(ABullet* UsedBullet)
{
	bulletPool.Add(UsedBullet);
}

void AFirearm::DetachParts(FName SocketName)
{
	TArray<AActor*> AttachedActors;
	this->GetAttachedActors(AttachedActors);

	for (AActor* Actor : AttachedActors)
	{
		if (Actor && Actor->GetAttachParentSocketName() == SocketName)
		{
			Actor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			return;
		}
	}	

	if (SocketName == "MagazineSocket")
	{
		MaxReloadedAmmo = 0;
		bIsMagazineAttached = false;
	}

}