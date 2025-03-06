#pragma once

#include "CoreMinimal.h"
#include "Monster/BossMonster.h"
#include "LaygoBossMonster.generated.h"

UCLASS()
class SHOOTINGGAME_API ALaygoBossMonster : public ABossMonster
{
	GENERATED_BODY()

public:
	ALaygoBossMonster();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime);

public:	
	UPROPERTY(EditAnywhere, Category = "FireBall")
	TSubclassOf<class AFireBall> FireBallClass;

	UFUNCTION(BlueprintCallable)
	void SpawnFireBall(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void SetPlayerActor(AActor* Actor);

private:
	AActor* PlayerActor;
	void RoateToPlayerActor(float DeltaTime);
};
