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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Animation")
	UAnimMontage* DeathMontage;

	UFUNCTION(BlueprintCallable)
	void SpawnFireBall(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void SetPlayerActor(AActor* Actor);

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void OnDeath() override;
	void DeathLogic();
	void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	AActor* PlayerActor;
	void RoateToPlayerActor(float DeltaTime);
	FTimerHandle OnDeathTimer;
	bool bIsPage2;
};
