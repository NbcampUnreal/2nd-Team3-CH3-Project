#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class SHOOTINGGAME_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	AMeleeWeapon();

protected:
	virtual void Attack() override;
	virtual void DealDamage(AActor* Enemy) override;
};
