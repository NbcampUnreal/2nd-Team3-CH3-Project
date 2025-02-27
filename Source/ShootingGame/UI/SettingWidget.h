#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWidget.generated.h"

class UButton;
class UUIManager;

UCLASS()
class SHOOTINGGAME_API USettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonAudioSetting;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonVideoSetting;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonKeyMapping;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonBack;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedAudioSetting();

	UFUNCTION()
	void OnClickedVideoSetting();

	UFUNCTION()
	void OnClickedKeyMapping();

	UFUNCTION()
	void OnClickedBack();

private:
	UUIManager* uiManager;
	bool isBinding;
};
