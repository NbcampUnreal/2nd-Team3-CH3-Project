#include "Monster/BaseMonster.h"
#include "MonsterAIController.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseMonster::ABaseMonster()
{
	PrimaryActorTick.bCanEverTick = false;

	HitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	HitCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HitCollision->SetupAttachment(GetMesh());

	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseMonster::OnHitCollisionOverlap);

	MaxHealth = 100.0f;
	Health = MaxHealth;

}

FName ABaseMonster::GetMonsterType()
{
	return MonsterType;
}

float ABaseMonster::GetHealth() const
{
	return Health;
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
		//UGameplayStatics::ApplyDamage(OtherActor, 10.0f, nullptr, this, UDamageType::StaticClass());
		UGameplayStatics::ApplyDamage(this, 10.0f, nullptr, this, UDamageType::StaticClass());
	}
}

float ABaseMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Monster Health decreased to: %f"), Health));

	if (Health <= 0.0f)
	{
		OnDeath();
	}

	return ActualDamage;
}

void ABaseMonster::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Monster OnDeath")));
	//UE_LOG(LogTemp, Warning, TEXT("Monster OnDeath"));

	DropItem();

	Destroy();
}

Item* ABaseMonster::DropItem()
{
	return nullptr;
}

