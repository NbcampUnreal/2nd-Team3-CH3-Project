#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class UButton;
class UUIManager;

UCLASS()
class SHOOTINGGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* GameStartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ArchievementsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedGameStart();

	UFUNCTION()
	void OnClickedSetting();

	UFUNCTION()
	void OnClickedAchievements();

	UFUNCTION()
	void OnClickedExit();

private:
	UUIManager* uiManager;
	bool isBinding;
};
