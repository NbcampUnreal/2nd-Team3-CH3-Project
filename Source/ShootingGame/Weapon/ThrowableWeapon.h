#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "ThrowableWeapon.generated.h"

class AThrowableProjectile;
class USphereComponent;

UCLASS()
class SHOOTINGGAME_API AThrowableWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	AThrowableWeapon();

	UFUNCTION(BlueprintPure, Category = "ThrowableWeapon")
	virtual int32 GetCurrentPossession() const;
	virtual int32 GetMaxPossession() const;
	UFUNCTION(BlueprintCallable, Category = "ThrowableWeapon")
	virtual void Throw();
	UFUNCTION(BlueprintCallable, Category = "ThrowableWeapon")
	virtual void Spawn();
	UFUNCTION(BlueprintCallable, Category = "ThrowableWeapon")
	virtual void AddPossession(int32 AddNumber);
	UFUNCTION(BlueprintCallable, Category = "ThrowableWeapon")
	virtual void ReadyToExplosion();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThrowableWeapon")
	USphereComponent* LocationCheck;
	UPROPERTY();
	AThrowableProjectile* ThrowableProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThrowableWeapon")
	TSubclassOf<AThrowableProjectile> ThrowableProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThrowableWeapon")
	int32 MaxPossession;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThrowableWeapon")
	int32 CurrentPossession;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ThrowableWeapon")
	float ThrowForce;

	bool bIsInHand;

	FTimerHandle ReloadTimerHandle;

};
