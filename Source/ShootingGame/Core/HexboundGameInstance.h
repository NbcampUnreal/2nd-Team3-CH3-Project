#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HexboundGameInstance.generated.h"

UENUM(BlueprintType)
enum class ELevel : uint8
{
	MenuLevel = 0,
	MainLevel
};

class AHexboundPlayerController;

UCLASS()
class SHOOTINGGAME_API UHexboundGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHexboundGameInstance();

	virtual void Init() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TArray<TSubclassOf<UUserWidget>> WidgetClasses;

	TSubclassOf<UUserWidget> InventoryWidget;
	TSubclassOf<UUserWidget> InGameWidget;

	UFUNCTION(BlueprintCallable)
	ELevel GetCurrentLevel();

	UFUNCTION(BlueprintCallable)
	void OpenLevel(ELevel level);


	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float MasterVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float SFXVolume = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float MusicVolume = 1.0f;

	// 볼륨 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetMasterVolume(float Volume);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetSFXVolume(float Volume);

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void SetMusicVolume(float Volume);

private:
	void InitUIMangerProperties();

	ELevel currentLevel;
	void SetCurrentLevel(ELevel level);
};
