// Copyright Epic Games, Inc. All Rights Reserved.

#include "Magazine.h"
#include "Weapon/Firearm.h"

AMagazine::AMagazine()
{
	PartsType = "Magazine";
	SocketNameForAttach = "MagazineSocket";
	MagazineCapacity = 30;
}

void AMagazine::AttachMagToWeapon(AFirearm* Weapon)
{
	if (Weapon)
	{
		this->AttachToComponent(Weapon->WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("MagazineSocket"));
	}
}

int32 AMagazine::GetMagazineCapacity() const
{
	return MagazineCapacity;
}