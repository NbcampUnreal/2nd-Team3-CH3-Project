#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

UCLASS()
class SHOOTINGGAME_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();

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

	// AIPerception 이벤트 핸들러
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	void CheckPlayerActor(AActor* sensedActor);

	void OnDetectedPlayer(UBlackboardComponent* BlackboardComp, AActor* Actor);
	void LosePlayer(UBlackboardComponent* BlackboardComp, AActor* Actor);

	void UpdateCharacterMovementSpeed(float NewSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Speed")
	float IdleSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Speed")
	float ChaseSpeed;
};
