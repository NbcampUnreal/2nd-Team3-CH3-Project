#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HexboundGameState.generated.h"

class AHexboundPlayerController;
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
	void OpenLevel(ELevel level);

private:
	void Init();
	AHexboundPlayerController* hexBoundPlayerController;
};
