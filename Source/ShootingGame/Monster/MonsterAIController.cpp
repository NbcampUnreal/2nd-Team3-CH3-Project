#include "Monster/MonsterAIController.h"
#include "Monster/BaseMonster.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionSystem.h"


AMonsterAIController::AMonsterAIController()
{
	// AIPerception 컴포넌트 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Sight 감각 설정 생성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Sight 설정
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 80.0f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	// AIPerception 컴포넌트에 Sight 설정 추가
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// Perception 업데이트 이벤트 바인딩
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnPerceptionUpdated);

	IdleSpeed = 200.0f;
	ChaseSpeed = 500.0f;
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (MonsterBT)
	{
		RunBehaviorTree(MonsterBT);
	}
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AMonsterAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
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
					//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("[Monster] Player detected by Sight!"));
					CheckPlayerActor(actor);
					break;
				}
				return;
			}
			else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>())		// 데미지 정보에 의해 찾았을때
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("[Monster] Player detected by Damage!"));
				//CheckPlayerActor(actor);
				return;
			}
		}
	}

}

void AMonsterAIController::CheckPlayerActor(AActor* sensedActor)
{
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	AActor* PlayerActor = Cast<AActor>(BlackboardComp->GetValueAsObject("PlayerActorKey"));

	if (!IsValid(PlayerActor))
	{
		OnDetectedPlayer(BlackboardComp, sensedActor);
	}
	else
	{
		LosePlayer(BlackboardComp, sensedActor);
	}

}


void AMonsterAIController::OnDetectedPlayer(UBlackboardComponent* BlackboardComp, AActor* Actor)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("[Monster] Player Detected!!")));
	BlackboardComp->SetValueAsObject("PlayerActorKey", Actor);

	// Blackboard에 PlayerDetected 값을 false로 설정
	BlackboardComp->SetValueAsBool("PlayerDetected", true);

	// 캐릭터의 이동 속도를 ChaseSpeed로 설정
	UpdateCharacterMovementSpeed(ChaseSpeed);
}

void AMonsterAIController::LosePlayer(UBlackboardComponent* BlackboardComp, AActor* Actor)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("[Monster] Lose Player!!")));

	// Player Actor Reference Set nullptr
	BlackboardComp->SetValueAsObject("PlayerActorKey", nullptr);

	// Blackboard에 PlayerDetected 값을 false로 설정
	BlackboardComp->SetValueAsBool("PlayerDetected", false);

	// Blackboard에 CanAttack 값을 false로 설정
	BlackboardComp->SetValueAsBool("CanAttack", false);

	// Blackboard에 CanAttack 값을 false로 설정
	BlackboardComp->SetValueAsBool("IsChase", false);

	// Blackboard에 CanAttack 값을 false로 설정
	BlackboardComp->SetValueAsBool("CanAttack", false);

	// 캐릭터의 이동 속도를 IdleSpeed으로 설정
	UpdateCharacterMovementSpeed(IdleSpeed);
}



void AMonsterAIController::UpdateCharacterMovementSpeed(float NewSpeed)
{
	// 컨트롤 중인 캐릭터 가져오기
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	if (ControlledCharacter && ControlledCharacter->GetCharacterMovement())
	{
		// 캐릭터 이동 속도 업데이트
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}