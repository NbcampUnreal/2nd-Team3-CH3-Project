#include "Monster/BaseMonster.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Monster/DamageIndicator/DamageIndicator.h"

// Sets default values
ABaseMonster::ABaseMonster()
{
	PrimaryActorTick.bCanEverTick = false;

	MonsterType = "BaseMonster";

	HandSocket = CreateDefaultSubobject<UChildActorComponent>(TEXT("HandSocket"));
	HandSocket->SetupAttachment(GetMesh(), TEXT("RightHand")); // 캐릭터 메시의 소켓 이름

	HitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	HitCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HitCollision->SetupAttachment(HandSocket);

	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseMonster::OnHitCollisionOverlap);

	MaxHealth = 100.0f;
	Health = MaxHealth;
	isDeath = false;

}

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();
}

FName ABaseMonster::GetMonsterType()
{
	return MonsterType;
}

float ABaseMonster::GetHealth() const
{
	return Health;
}

bool ABaseMonster::IsCharacterDeath()
{
	return isDeath;
}


void ABaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseMonster::Attack()
{

}

void ABaseMonster::OnHitCollisionOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!")));

		UGameplayStatics::ApplyDamage(OtherActor, 10.0f, nullptr, this, UDamageType::StaticClass());
	}
}

float ABaseMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float ArmorReduction = FMath::RandRange(0.0f, Armor);
	float ActualDamage = Damage - ArmorReduction;

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Monster Health decreased to: %f"), Health));

	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);

	if (Health > 0.0f)
	{
		OnTakeDamage();
		CreateDamageIndicator(ActualDamage);
	}
	else
	{
		OnDeath();
	}

	return ActualDamage;
}

void ABaseMonster::OnTakeDamage()
{
	if (ReactionHitMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(ReactionHitMontage);
		}
	}
}

void ABaseMonster::OnDeath()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Monster OnDeath")));
	isDeath = true;
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	GetMesh()->Stop();
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();

	SetLifeSpan(5.0f);
}

Item* ABaseMonster::DropItem()
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Drop Item!!!")));
	return nullptr;
}


void ABaseMonster::CreateDamageIndicator(float Amount)
{
	if (DamageIndicatorClass)
	{
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 100); // 몬스터 머리 위에 표시
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParameters;
		ADamageIndicator* DamageIndicator = GetWorld()->SpawnActor<ADamageIndicator>
			(this->DamageIndicatorClass,
				SpawnLocation,
				SpawnRotation,
				SpawnParameters
			);

		if (DamageIndicator)
		{
			DamageIndicator->SetDamage(Amount);
		}
	}
}
