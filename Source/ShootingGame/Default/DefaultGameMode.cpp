// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "GameFramework/HUD.h"
#include "../Character/MyCharacter.h"
#include "../Character/MyPlayerController.h"

ADefaultGameMode::ADefaultGameMode()
{
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/DefaultCharacter/Blueprints/BP_DefaultCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	//캐릭터 테스트를 위한 코드
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/Blueprints/BP_MyCharacter"));
	if (PlayerPawnBPClass.Succeeded())
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Characters/Blueprints/BP_MyPlayerController"));
	if (PlayerControllerBPClass.Succeeded())
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}
