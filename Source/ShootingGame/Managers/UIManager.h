#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManager.generated.h"

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	//Intro = 0,
	MenuBase = 0,
	MainMenu,
	Setting,
	AudioSetting,
	VideoSetting,
	KeyMapping,
	Archivements,

	InGameBase,

	Pause,
	Inventory,

	GameOver
};


// class AHexboundPlayerController;

UCLASS()
class SHOOTINGGAME_API UUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UUIManager();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	TSubclassOf<UUserWidget> GetWidgetClasse(EHUDState state);
	void SetWidgetClasses(TArray<TSubclassOf<UUserWidget>> widgets);
	TArray<TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TMap<EHUDState, UUserWidget*> WidgetInstances;

	//UWorld* World;
	//AHexboundPlayerController* HexboundController;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetUIState(EHUDState state);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowUI(EHUDState state);
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideUI(EHUDState state);
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideAllUI();

	void ShowInventory(bool isShow);
	UUserWidget* InventoryWidgetInstance;

private:

};
