#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "HexboundGameMode.generated.h"

UCLASS()
class SHOOTINGGAME_API AHexboundGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AHexboundGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
};
