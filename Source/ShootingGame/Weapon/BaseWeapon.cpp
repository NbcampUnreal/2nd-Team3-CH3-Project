// Copyright Epic Games, Inc. All Rights Reserved.

#include "BaseWeapon.h"
#include "Default/DefaultCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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

	bIsCooltimeEnd = true;	// 발사 간격 체크

	WeaponType = "";		// 무기 종류
	Damage = 0.0f;			// 데미지
	AttackRate = 5.0f;		// 발사 간격
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

void ABaseWeapon::DealDamage(AActor* Enemy) // Attack또는 프로젝타일에서 액터 판별 후 호출됨
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
