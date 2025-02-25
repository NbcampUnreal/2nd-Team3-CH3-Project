#pragma once

#include "CoreMinimal.h"
#include "Weapon/MainWeapon.h"
#include "SniperRifle.generated.h"

UCLASS()
class SHOOTINGGAME_API ASniperRifle : public AMainWeapon
{
	GENERATED_BODY()
	
public:
	ASniperRifle();

	virtual void EquipParts(AParts* Parts) override;
};
