// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireBall.generated.h"

UCLASS()
class SHOOTINGGAME_API AFireBall : public AActor
{
	GENERATED_BODY()
	
public:	
	AFireBall();

	void InitializeFireBall(float InSpeed, float InDamage, FVector InDirection);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Obstacle|Component")
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle|Component")
	UPrimitiveComponent* Collision;

	UFUNCTION()
	virtual void OnFireBallOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, Category = "FireBall")
	float Speed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "FireBall")
	float Damage = 10.0f;

	FVector MoveDirection;
};
