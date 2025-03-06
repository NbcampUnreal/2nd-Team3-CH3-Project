#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BossMonsterAIC.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

UCLASS()
class SHOOTINGGAME_API ABossMonsterAIC : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossMonsterAIC();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BT")
	UBehaviorTree* MonsterBT;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	// Sight 감각 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	// Damage 감각 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Damage* DamageConfig;

	UFUNCTION()
	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	virtual void OnBossBattleStart();

	UBlackboardComponent* BlackboardComp;
};
