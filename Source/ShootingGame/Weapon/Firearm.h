#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "Firearm.generated.h"

class AMagazine;
class ABullet;
class AParts;

UCLASS()
class SHOOTINGGAME_API AFirearm : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	AFirearm();

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	UStaticMesh* BulletMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Component")
	USoundBase* ReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Component")
	USoundBase* EmptyAmmoSound;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void EquipParts(AParts* Parts);
	UFUNCTION()
	virtual void ReturnBulletToPool(ABullet* UsedBullet);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void AddAmmo(int32 AmmoToAdd);
	UFUNCTION(BlueprintPure, Category = "Weapon")
	virtual int32 GetCurrentAmmoValue() const;
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void DetachParts(FName SocketName);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Attack() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Bullet")
	float BulletSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Bullet")
	int32 MaxAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Bullet")
	int32 CurrentAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Bullet")
	int32 MaxReloadedAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Bullet")
	int32 ReloadedAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Bullet")
	float ReloadTime;

	bool bIsLoadingComplete;
	TSubclassOf<ABullet> BulletClass;
	FTimerHandle ReloadTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();
	UFUNCTION()
	virtual void Fire();

private:
	TArray<ABullet*> bulletPool;

};
