#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HexboundPlayerController.generated.h"

UCLASS()
class SHOOTINGGAME_API AHexboundPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHexboundPlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void ShowCursor(bool isShow);
	
protected:
	

};
