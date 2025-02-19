// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseWeapon.h"
#include "Default/DefaultCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "HAL/PlatformTime.h"
#include "Kismet/GameplayStatics.h"

ABaseWeapon::ABaseWeapon()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);


	USkeletalMesh* Weapon = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Weapons/Firearms/Rifle/SK_AR4.SK_AR4"));
	if (Weapon)
	{
		WeaponMesh->SetSkeletalMesh(Weapon);
	}

	PrimaryActorTick.bCanEverTick = false;

	CurrentAttackTime = 0.0f;
	LastAttackTime = 0.0f;

	bIsCooltimeEnd = true;

	WeaponType = "";
	Damage = 0.0f;

	AttackRate = 5.0f;
}


float ABaseWeapon::GetDamageValue() const
{
	return Damage;
}

void ABaseWeapon::Attack()
{
	if (bIsCooltimeEnd)
	{
		bIsCooltimeEnd = false;
		if (AttackSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound, FVector::ZeroVector);
		}
		GetWorldTimerManager().SetTimer(attackRateTimerHandle, [this]() {bIsCooltimeEnd = true; }, AttackRate, false);
	}
}

void ABaseWeapon::DealDamage(AActor* Enemy)
{
	if (Enemy)
	{
		UGameplayStatics::ApplyDamage(Enemy, Damage, nullptr, this, UDamageType::StaticClass());
	}
}

void ABaseWeapon::AttachWeaponToCharacter(ADefaultCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		this->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
	}
}
