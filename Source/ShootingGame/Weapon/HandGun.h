#pragma once

#include "CoreMinimal.h"
#include "Weapon/SubWeapon.h"
#include "HandGun.generated.h"

UCLASS()
class SHOOTINGGAME_API AHandGun : public ASubWeapon
{
	GENERATED_BODY()

public:
	AHandGun();
	
	virtual void EquipParts(AParts* Parts) override;
};
