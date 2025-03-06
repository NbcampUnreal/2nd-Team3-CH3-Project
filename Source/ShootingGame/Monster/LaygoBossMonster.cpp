#include "Monster/LaygoBossMonster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster/Skill/FireBall.h"

ALaygoBossMonster::ALaygoBossMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	PlayerActor = nullptr;
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

