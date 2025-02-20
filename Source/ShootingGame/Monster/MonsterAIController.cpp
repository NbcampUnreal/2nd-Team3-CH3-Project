#include "Monster/MonsterAIController.h"
#include "Monster/BaseMonster.h"
#include "Navigation/PathFollowingComponent.h"

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AMonsterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

}



