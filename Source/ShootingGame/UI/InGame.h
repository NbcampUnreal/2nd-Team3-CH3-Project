#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGame.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class SHOOTINGGAME_API UInGame : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateAmmo(int32 LoadedAmmo, int32 ReaminingAmmo);
	UFUNCTION(BlueprintCallable, Category = "UI")
	void PrintCurrentWeapon(FName WeaponType, UTexture2D* WeaponImage);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* loadedAmmoText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* remainingAmmoText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* currentWeapon;
	UPROPERTY(meta = (BindWidget))
	UImage* WeaponUIImage;
	UPROPERTY()
	UTexture2D* WeaponImageTexture;
};
