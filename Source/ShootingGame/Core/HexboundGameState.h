#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HexboundGameState.generated.h"

class AHexboundPlayerController;

UCLASS()
class SHOOTINGGAME_API AHexboundGameState : public AGameState
{
	GENERATED_BODY()

public:
	AHexboundGameState();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TArray<TSubclassOf<UUserWidget>> WidgetClasses;
	
	UFUNCTION(BlueprintCallable)
	void OnGameStart();

private:
	void InitUIMangerProperties();
	AHexboundPlayerController* HexBoundPlayerController;
};
