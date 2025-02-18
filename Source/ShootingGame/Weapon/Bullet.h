#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class AFirearm;
class UProjectileMovementComponent;

UCLASS()
class SHOOTINGGAME_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

	void ActivateBullet(FVector SpawnLocation, FRotator SpawnRotation, float Speed);
	void SetBulletMesh(UStaticMesh* BulletMesh);
	void SetBulletSpeed(float Speed);
	void SetFirearm(AFirearm* Firearm);

protected:



private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* bulletMesh;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* projectileMovement;

	AFirearm* gun;

	UFUNCTION()
	void onHit(
		UPrimitiveComponent* HitComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, 
		const FHitResult& Hit);
};
