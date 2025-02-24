#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HexboundGameInstance.generated.h"

UENUM(BlueprintType)
enum class ELevel : uint8
{
	MenuLevel = 0,
	MainLevel,
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
	void SetCurrentLevel(ELevel level);

private:
	void InitUIMangerProperties();
	ELevel currentLevel;

};
