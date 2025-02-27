#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UButton;
class UUIManager;

UCLASS()
class SHOOTINGGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonClose;

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnClickedClose();

private:
	UUIManager* uiManager;
	bool isBinding;
	
};
