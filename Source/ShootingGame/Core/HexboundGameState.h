#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HexboundGameState.generated.h"

class AMyPlayerController;
enum class ELevel : uint8;

UCLASS()
class SHOOTINGGAME_API AHexboundGameState : public AGameState
{
	GENERATED_BODY()

public:
	AHexboundGameState();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void OnGameStart();

	UFUNCTION(BlueprintCallable)
	void OnGameOver();

	void AddScore(int amount);

	int KillCount;
	int Score;
	float PlayTime;

private:
	void Init();
	AMyPlayerController* myPlayerController;
};
