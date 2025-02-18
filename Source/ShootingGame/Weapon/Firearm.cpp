// Copyright Epic Games, Inc. All Rights Reserved.

#include "Firearm.h"
#include "..\..\Source\ShootingGame\Weapon/Bullet.h"
#include "..\..\Source\ShootingGame\Weapon/Magazine.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

AFirearm::AFirearm()
{
	PrimaryActorTick.bCanEverTick = false;

	BulletMesh = nullptr;
	BulletSpeed = 3000.0f;
	MaxAmmo = 0;
	CurrentAmmo = 0;
	MaxReloadedAmmo = 0;
	ReloadedAmmo = 0;
	ReloadTime = 1.0f;
	bIsLoadingComplete = true;
}

void AFirearm::Attack()
{
	if (bIsLoadingComplete)
	{
		if (CurrentAmmo > 0 && bIsCooltimeEnd)
		{
			Super::Attack();

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

			CurrentAmmo--;
		}

		if (CurrentAmmo <= 0 && bIsCooltimeEnd)
		{
			if (EmptyAmmoSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), EmptyAmmoSound, FVector::ZeroVector);
			}
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
	//HUDUpdate
}

void AFirearm::Reload()
{
	bIsLoadingComplete = false;
	GetWorldTimerManager().SetTimer(
		ReloadTimerHandle, 
		[this]() {
			bIsLoadingComplete = true; 
			int32 ReloadValue = MaxReloadedAmmo - ReloadedAmmo;
			CurrentAmmo -= ReloadValue;
		}, 
		ReloadTime, 
		false
	);

	//HUDUpdate
}

void AFirearm::EquipMagazine(AMagazine* Magazine)
{
	if (Magazine)
	{
		MaxReloadedAmmo = Magazine->GetMagazineCapacity();
		Magazine->AttachMagToWeapon(this);
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