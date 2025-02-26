#include "Monster/MonsterAIController.h"
#include "Monster/BaseMonster.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

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

	// AIPerception 컴포넌트에 Sight 설정 추가
	AIPerceptionComponent->ConfigureSense(*SightConfig);
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
		if (actor->ActorHasTag("Player"))
		{
			CheckPlayerActor(actor);
			break;
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

	// 캐릭터의 이동 속도를 200으로 설정
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

	// 캐릭터의 이동 속도를 500으로 설정
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