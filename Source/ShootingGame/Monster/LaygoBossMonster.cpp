#include "Monster/LaygoBossMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster/Skill/FireBall.h"
#include "Components/WidgetComponent.h"
#include "Monster/LaygoBossAIC.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Monster/DamageIndicator/WidgetHealthBar.h"
#include "Core/HexboundGameState.h"

ALaygoBossMonster::ALaygoBossMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerActor = nullptr;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	bIsPage2 = false;
}

void ALaygoBossMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaygoBossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RoateToPlayerActor(DeltaTime);
}

void ALaygoBossMonster::SpawnFireBall(AActor* TargetActor)
{
	if (FireBallClass)
	{
		// 오른손 소켓 위치 가져오기
		FVector HandLocation = GetMesh()->GetSocketLocation(TEXT("RightHand"));

		// 플레이어 위치 가져오기
		if (!TargetActor) return;

		// FireBall을 플레이어 방향으로 발사
		FVector Direction = (TargetActor->GetActorLocation() - HandLocation).GetSafeNormal();

		// FireBall 생성
		FRotator FireBallRotation = Direction.Rotation();
		AFireBall* FireBall = GetWorld()->SpawnActor<AFireBall>(FireBallClass, HandLocation, FireBallRotation);
		if (FireBall)
		{
			FireBall->InitializeFireBall(1500.0f, 20.0f, Direction);
		}
	}
}

void ALaygoBossMonster::SetPlayerActor(AActor* Actor)
{
	PlayerActor = Actor;
}

float ALaygoBossMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float actualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float HealthRate = Health / MaxHealth;

	if (!bIsPage2)
	{
		if (HealthRate <= 0.5f)
		{
			if (ALaygoBossAIC* AIController = Cast<ALaygoBossAIC>(GetController()))
			{
				AIController->SetBossPage(2);
				bIsPage2 = true;
			}
		}
	}

	if (UWidgetHealthBar* widgetHealthBar = Cast<UWidgetHealthBar>(OverheadWidget->GetUserWidgetObject()))
	{
		widgetHealthBar->HealthBar->SetPercent(HealthRate);
	}

	return actualDamage;
}

void ALaygoBossMonster::OnDeath()
{
	isDeath = true;

	ALaygoBossAIC* AIC = Cast<ALaygoBossAIC>(GetController());
	if (AIC)
	{
		AIC->UnPossess();
	}

	if (DeathMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ALaygoBossMonster::OnDeathMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DeathMontage);
			AnimInstance->Montage_Play(DeathMontage);
		}
	}

	FTimerHandle DestroyParticleTimerHandle;
	GetWorld()->GetTimerManager().SetTimer
	(
		DestroyParticleTimerHandle,
		[this]() 
		{
			DeathLogic();
		},
		DeathMontage->GetPlayLength() - 0.3,
		false
	);

}

void ALaygoBossMonster::DeathLogic()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (AHexboundGameState* GameState = Cast<AHexboundGameState>(GetWorld()->GetGameState()))
	{
		GameState->OnGameOver();
	}

	Destroy();
}

void ALaygoBossMonster::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

	if (Montage == DeathMontage)
	{
		//GetWorld()->GetTimerManager().SetTimer(
		//	FTimerHandle(),
		//	this,
		//	&AActor::Destroy,
		//	3.0f, // 3초 후 삭제
		//	false
		//);
	}

}

void ALaygoBossMonster::RoateToPlayerActor(float DeltaTime)
{
	if (PlayerActor)
	{
		// Boss 위치 & Player 위치 가져오기
		FVector BossLocation = GetActorLocation();
		FVector PlayerLocation = PlayerActor->GetActorLocation();

		// 플레이어를 향하는 방향 계산
		FRotator LookAtRotation = (PlayerLocation - BossLocation).Rotation();

		// Y축 회전만 적용 (Pitch와 Roll을 0으로 설정)
		LookAtRotation.Pitch = 0.0f;
		LookAtRotation.Roll = 0.0f;

		// 부드러운 회전 (보스가 자연스럽게 회전하도록 설정)
		float RotationSpeed = 5.0f; // 회전 속도
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotation, DeltaTime, RotationSpeed);
		SetActorRotation(NewRotation);
	}
}

