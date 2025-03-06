#include "Monster/BossMonsterAIC.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionSystem.h"


ABossMonsterAIC::ABossMonsterAIC()
{
	// AIPerception ������Ʈ ����
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Sight ���� ���� ����
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Sight ����
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1600.0f;
	SightConfig->PeripheralVisionAngleDegrees = 80.0f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Damage ����
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	// AIPerception ������Ʈ�� Sight ���� �߰�
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// Perception ������Ʈ �̺�Ʈ ���ε�
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABossMonsterAIC::OnPerceptionUpdated);
}

void ABossMonsterAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (MonsterBT)
	{
		RunBehaviorTree(MonsterBT);
		BlackboardComp = GetBlackboardComponent();
	}
}

void ABossMonsterAIC::BeginPlay()
{
	Super::BeginPlay();
}

void ABossMonsterAIC::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

}

void ABossMonsterAIC::OnBossBattleStart()
{
}
