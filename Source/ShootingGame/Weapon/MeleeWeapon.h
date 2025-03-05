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

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	USoundBase* HitSound;

protected:
	virtual void Attack() override;
	
	UFUNCTION()
	virtual void OnEnemyOverlap(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyindex, 
		bool bFromSweep, 
		const FHitResult& Sweep);
};
