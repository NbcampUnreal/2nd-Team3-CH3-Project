#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UButton;
class UTextBlock;
class UUIManager;

UCLASS()
class SHOOTINGGAME_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextScore;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextKillCount;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextPlayTime;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonHome;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonRestart;
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonExit;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickHomeButton();

	UFUNCTION()
	void OnClickRestartButton();

	UFUNCTION()
	void OnClickExitButton();

private:
	UUIManager* uiManager;
	bool isBinding;

};
