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
	// AIPerception ������Ʈ ����
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Sight ���� ���� ����
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Sight ����
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1000.0f;
	SightConfig->PeripheralVisionAngleDegrees = 80.0f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// AIPerception ������Ʈ�� Sight ���� �߰�
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// Perception ������Ʈ �̺�Ʈ ���ε�
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

	// Blackboard�� PlayerDetected ���� false�� ����
	BlackboardComp->SetValueAsBool("PlayerDetected", true);

	// ĳ������ �̵� �ӵ��� 200���� ����
	UpdateCharacterMovementSpeed(ChaseSpeed);
}

void AMonsterAIController::LosePlayer(UBlackboardComponent* BlackboardComp, AActor* Actor)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("[Monster] Lose Player!!")));

	// Player Actor Reference Set nullptr
	BlackboardComp->SetValueAsObject("PlayerActorKey", nullptr);

	// Blackboard�� PlayerDetected ���� false�� ����
	BlackboardComp->SetValueAsBool("PlayerDetected", false);

	// Blackboard�� CanAttack ���� false�� ����
	BlackboardComp->SetValueAsBool("CanAttack", false);

	// Blackboard�� CanAttack ���� false�� ����
	BlackboardComp->SetValueAsBool("IsChase", false);

	// Blackboard�� CanAttack ���� false�� ����
	BlackboardComp->SetValueAsBool("CanAttack", false);

	// ĳ������ �̵� �ӵ��� 500���� ����
	UpdateCharacterMovementSpeed(IdleSpeed);
}



void AMonsterAIController::UpdateCharacterMovementSpeed(float NewSpeed)
{
	// ��Ʈ�� ���� ĳ���� ��������
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	if (ControlledCharacter && ControlledCharacter->GetCharacterMovement())
	{
		// ĳ���� �̵� �ӵ� ������Ʈ
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}