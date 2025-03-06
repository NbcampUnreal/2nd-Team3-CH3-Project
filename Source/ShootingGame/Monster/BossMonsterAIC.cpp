#include "Monster/BossMonsterAIC.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionSystem.h"


ABossMonsterAIC::ABossMonsterAIC()
{
	// AIPerception 컴포넌트 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// Sight 감각 설정 생성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	// Sight 설정
	SightConfig->SightRadius = 800.0f;
	SightConfig->LoseSightRadius = 1600.0f;
	SightConfig->PeripheralVisionAngleDegrees = 80.0f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Damage 설정
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	// AIPerception 컴포넌트에 Sight 설정 추가
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*DamageConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// Perception 업데이트 이벤트 바인딩
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
