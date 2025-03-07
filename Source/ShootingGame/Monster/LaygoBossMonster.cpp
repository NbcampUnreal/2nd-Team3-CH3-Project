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
		// ������ ���� ��ġ ��������
		FVector HandLocation = GetMesh()->GetSocketLocation(TEXT("RightHand"));

		// �÷��̾� ��ġ ��������
		if (!TargetActor) return;

		// FireBall�� �÷��̾� �������� �߻�
		FVector Direction = (TargetActor->GetActorLocation() - HandLocation).GetSafeNormal();

		// FireBall ����
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
		//	3.0f, // 3�� �� ����
		//	false
		//);
	}

}

void ALaygoBossMonster::RoateToPlayerActor(float DeltaTime)
{
	if (PlayerActor)
	{
		// Boss ��ġ & Player ��ġ ��������
		FVector BossLocation = GetActorLocation();
		FVector PlayerLocation = PlayerActor->GetActorLocation();

		// �÷��̾ ���ϴ� ���� ���
		FRotator LookAtRotation = (PlayerLocation - BossLocation).Rotation();

		// Y�� ȸ���� ���� (Pitch�� Roll�� 0���� ����)
		LookAtRotation.Pitch = 0.0f;
		LookAtRotation.Roll = 0.0f;

		// �ε巯�� ȸ�� (������ �ڿ������� ȸ���ϵ��� ����)
		float RotationSpeed = 5.0f; // ȸ�� �ӵ�
		FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), LookAtRotation, DeltaTime, RotationSpeed);
		SetActorRotation(NewRotation);
	}
}

