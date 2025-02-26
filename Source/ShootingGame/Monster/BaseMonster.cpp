#include "Monster/BaseMonster.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/ChildActorComponent.h"

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

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Overlap!!")));
		UGameplayStatics::ApplyDamage(OtherActor, 10.0f, nullptr, this, UDamageType::StaticClass());

		//UGameplayStatics::ApplyDamage(this, 10.0f, nullptr, this, UDamageType::StaticClass());
	}
}

float ABaseMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Monster Health decreased to: %f"), Health));

	if (Health <= 0.0f)
	{
		OnDeath();
	}
	else
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

	return ActualDamage;
}

void ABaseMonster::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Monster OnDeath")));
	isDeath = true;

	AMonsterAIController* AIController = Cast<AMonsterAIController>(GetController());
	if (AIController)
	{
		AIController->UnPossess(); // AI 컨트롤러 비활성화
	}

	if (DeathMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &ABaseMonster::OnDeathMontageEnded);

			AnimInstance->Montage_Play(DeathMontage);
			AnimInstance->Montage_SetEndDelegate(EndDelegate);
			return;
		}
	}
}

Item* ABaseMonster::DropItem()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Drop Item!!!")));
	return nullptr;
}

void ABaseMonster::OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == DeathMontage)
	{
		DropItem();

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Try Destroy Monster")));
		Destroy();
	}
}
