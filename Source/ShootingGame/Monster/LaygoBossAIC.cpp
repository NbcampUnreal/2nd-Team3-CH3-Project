#include "Monster/LaygoBossAIC.h"
#include "Monster/LaygoBossMonster.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionSystem.h"

#include "BehaviorTree/BlackboardComponent.h"

ALaygoBossAIC::ALaygoBossAIC()
{
	SightConfig->SightRadius = 1600.0f;
	SightConfig->LoseSightRadius = 3000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
}

void ALaygoBossAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void ALaygoBossAIC::BeginPlay()
{
	Super::BeginPlay();

}

void ALaygoBossAIC::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerceptionComponent->GetActorsPerception(actor, Info);

		for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())			// 시각 정보에 의해 찾았을때
			{
				if (actor->ActorHasTag("Player"))
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("[Boss Monster] Player detected by Sight!"));

					if (BlackboardComp)
					{
						BlackboardComp->SetValueAsObject("PlayerActor", actor);

						ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
						if (ALaygoBossMonster* BossMonster = Cast<ALaygoBossMonster>(ControlledCharacter))
						{
							BossMonster->SetPlayerActor(actor);
						}

					}

					//AActor* PlayerActor = Cast<AActor>(BlackboardComp->GetValueAsObject("PlayerActor"));
					//CheckPlayerActor(actor);
					break;
				}
				return;
			}
			else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>())		// 데미지 정보에 의해 찾았을때
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("[Boss Monster] Player detected by Damage!"));
				//CheckPlayerActor(actor);
				return;
			}
		}
	}
}

void ALaygoBossAIC::OnBossBattleStart()
{

}
