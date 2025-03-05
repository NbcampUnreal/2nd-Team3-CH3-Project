// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageActor.generated.h"

UCLASS()
class SHOOTINGGAME_API ADamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageActor();

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* WidgetComponent;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UDamageIndicator> DamageTextWidgetClass;

};
