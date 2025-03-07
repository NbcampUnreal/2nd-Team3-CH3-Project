#include "Monster/BossMonster.h"

ABossMonster::ABossMonster()
{
	PrimaryActorTick.bCanEverTick = false;

	MaxHealth = 1000.0f;
	Health = MaxHealth;
	Armor = 10.0f;
	isDeath = false;
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();
	
}


float ABossMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float ArmorReduction = FMath::RandRange(0.0f, Armor);
	float ActualDamage = Damage - ArmorReduction;

	Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);

	if (Health > 0.0f)
	{
		OnTakeDamage();
	}
	else
	{
		if (!isDeath)
		{
			OnDeath();
		}
	}

	return ActualDamage;
}

void ABossMonster::OnTakeDamage()
{
	//if (ReactionHitMontage)
	//{
	//	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//	if (AnimInstance)
	//	{
	//		AnimInstance->Montage_Play(ReactionHitMontage);
	//	}
	//}
}

void ABossMonster::OnDeath()
{
	isDeath = true;
}

void ABossMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
