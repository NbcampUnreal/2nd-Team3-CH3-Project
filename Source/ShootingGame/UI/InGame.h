#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGame.generated.h"

class UTextBlock;

UCLASS()
class SHOOTINGGAME_API UInGame : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateAmmoWidget(int32 LoadedAmmo, int32 ReaminingAmmo);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* loadedAmmoText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* remainingAmmoText;
};
