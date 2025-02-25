#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class ADefaultCharacter;
class UNiagaraSystem;

UCLASS()
class SHOOTINGGAME_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Component")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Component")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Component")
	UNiagaraSystem* AttackNiagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Component")
	USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponType;

	UFUNCTION()
	virtual float GetDamageValue() const;
	UFUNCTION()
	virtual void DealDamage(AActor* Enemy);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Attack();

protected:
	// 공격관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|WeaponStat")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|WeaponStat")
	float AttackRate;
	UPROPERTY(BlueprintReadOnly, Category = "WeaponStat")
	bool bIsCooltimeEnd;

	virtual void AttachWeaponToCharacter(ADefaultCharacter* PlayerCharacter);

private:

	FTimerHandle attackRateTimerHandle;
};