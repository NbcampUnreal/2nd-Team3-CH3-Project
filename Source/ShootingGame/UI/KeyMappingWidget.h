#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyMappingWidget.generated.h"

class UButton;
class UUIManager;

UCLASS()
class SHOOTINGGAME_API UKeyMappingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonBack;

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnClickedBack();

private:
	UUIManager* uiManager;
	bool isBinding;
};
