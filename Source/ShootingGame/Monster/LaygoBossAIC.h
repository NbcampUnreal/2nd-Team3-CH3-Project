#pragma once

#include "CoreMinimal.h"
#include "Monster/BossMonsterAIC.h"
#include "LaygoBossAIC.generated.h"


UCLASS()
class SHOOTINGGAME_API ALaygoBossAIC : public ABossMonsterAIC
{
	GENERATED_BODY()

public:
	ALaygoBossAIC();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

	void OnBossBattleStart() override;
	
};
