// Copyright Epic Games, Inc. All Rights Reserved.

#include "Magazine.h"
#include "..\..\Source\ShootingGame\Weapon\Firearm.h"

AMagazine::AMagazine()
{
	MagazineCapacity = 30;
}

void AMagazine::AttachMagToWeapon(AFirearm* Weapon)
{
	if (Weapon)
	{
		Weapon->EquipMagazine(this);
		this->AttachToComponent(Weapon->WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MagazineSocket"));
	}
}

int32 AMagazine::GetMagazineCapacity() const
{
	return MagazineCapacity;
}