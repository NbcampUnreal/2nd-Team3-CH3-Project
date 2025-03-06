// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossMonster.generated.h"

UCLASS()
class SHOOTINGGAME_API ABossMonster : public ACharacter
{
	GENERATED_BODY()

public:
	ABossMonster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
