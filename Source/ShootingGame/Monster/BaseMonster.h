#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseMonster.generated.h"

class UChildActorComponent;
class Item;

UCLASS()
class SHOOTINGGAME_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseMonster();

	FName GetMonsterType();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Component")
	UChildActorComponent* HandSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Component")
	UPrimitiveComponent* HitCollision;

	UFUNCTION(BlueprintPure, Category = "Monster|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	bool IsCharacterDeath();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	FName MonsterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Health")
	float Health;

	virtual void Attack();

	UFUNCTION()
	virtual void OnHitCollisionOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Monster|Health")
	virtual void OnDeath();

	virtual Item* DropItem();

	/** ���� ������ �޾����� �ִϸ��̼� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Animation")
	UAnimMontage* ReactionHitMontage;

	/** ���� ��� �ִϸ��̼� */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster|Animation")
	UAnimMontage* DeathMontage;

	/** ��� �� ȣ��� �Լ� */
	UFUNCTION()
	void OnDeathMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool isDeath;
};
