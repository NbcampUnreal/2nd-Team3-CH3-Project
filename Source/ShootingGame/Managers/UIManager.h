#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	//Intro = 0,
	MainMenu = 0,
	Setting,
	AudioSetting,
	VideoSetting,
	KeyMapping,
	Archivements,
	InGame,
	Pause
};


class AHexboundPlayerController;

UCLASS()
class SHOOTINGGAME_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UUIManager();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UWorld* World;
	AHexboundPlayerController* HexboundController;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowUI(EHUDState state);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TArray<TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* CurrentWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TArray<UUserWidget*> WidgetInstances;

};
