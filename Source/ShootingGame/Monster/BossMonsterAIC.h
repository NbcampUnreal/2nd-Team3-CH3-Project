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

	// Sight ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	// Damage ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UAISenseConfig_Damage* DamageConfig;

	UFUNCTION()
	virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	virtual void OnBossBattleStart();

	UBlackboardComponent* BlackboardComp;
};
