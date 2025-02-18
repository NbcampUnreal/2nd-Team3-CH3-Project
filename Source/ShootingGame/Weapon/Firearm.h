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

	UFUNCTION()
	virtual void EquipParts(AParts* Parts);
	UFUNCTION()
	virtual void ReturnBulletToPool(ABullet* UsedBullet);
	UFUNCTION()
	virtual void AddAmmo(int32 AmmoToAdd);
	UFUNCTION()
	virtual int32 GetCurrentAmmoValue() const;
	UFUNCTION()
	virtual void DetachParts(FName SocketName);
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float BulletSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	int32 MaxAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	int32 CurrentAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	int32 MaxReloadedAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	int32 ReloadedAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	float ReloadTime;
	bool bIsLoadingComplete;
	TSubclassOf<ABullet> BulletClass;
	FTimerHandle ReloadTimerHandle;

	UFUNCTION()
	virtual void Attack() override;
	UFUNCTION()
	virtual void Reload();
	UFUNCTION()
	virtual void Fire();

private:
	TArray<ABullet*> bulletPool;

};
