#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

UCLASS()
class SHOOTINGGAME_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void ShowIntroUI();
	void ShowMainUI();
	void ShowSettingUI();
	void ShowInGameUI();
	
};
