#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HexboundGameState.generated.h"

UCLASS()
class SHOOTINGGAME_API AHexboundGameState : public AGameState
{
	GENERATED_BODY()

public:
	AHexboundGameState();

	virtual void BeginPlay() override;
	
};
