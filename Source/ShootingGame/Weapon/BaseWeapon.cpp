// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseWeapon.h"
#include "..\..\Source\ShootingGame\Default\DefaultCharacter.h"
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

	USkeletalMesh* Weapon = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/LP_LiteWeapons_JC/Models/SK_WeaponA.SK_WeaponA"));
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

	AttackRate = 1.0f;
	AttackDistance = 10.0f;
}


float ABaseWeapon::GetDamageValue() const
{
	return Damage;
}

void ABaseWeapon::Attack()
{
	CurrentAttackTime = (float)FPlatformTime::Seconds();
	float AttackElapse = CurrentAttackTime - LastAttackTime;
		
	bIsCooltimeEnd = AttackElapse >= AttackRate ? true : false;
	
	if (bIsCooltimeEnd)
	{
		if (AttackSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AttackSound, FVector::ZeroVector);
		}

		LastAttackTime = (float)FPlatformTime::Seconds();
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
