// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"

ADefaultGameMode::ADefaultGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/DefaultCharacter/Blueprints/BP_DefaultCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
