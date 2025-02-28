#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioSettingWidget.generated.h"

class UButton;
class USlider;
class UUIManager;
class UHexboundGameInstance;

UCLASS()
class SHOOTINGGAME_API UAudioSettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	USlider* SliderMasterVolume;

	UPROPERTY(meta = (BindWidget))
	USlider* SliderSFXVolume;

	UPROPERTY(meta = (BindWidget))
	USlider* SliderMusicVolume;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonBack;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnMasterVolumeChanged(float Value);

	UFUNCTION()
	void OnSFXVolumeChanged(float Value);

	UFUNCTION()
	void OnMusicVolumeChanged(float Value);

	UFUNCTION()
	void OnClickedBack();

private:
	UUIManager* uiManager;
	UHexboundGameInstance* hexboundGameInstance;

	bool isBinding;
};
