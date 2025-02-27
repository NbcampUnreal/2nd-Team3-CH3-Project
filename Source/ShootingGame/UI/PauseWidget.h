#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton;
class UUIManager;

UCLASS()
class SHOOTINGGAME_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonSetting;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonResume;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonHome;

protected:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnClickedSetting();
	UFUNCTION()
	void OnClickedResume();
	UFUNCTION()
	void OnClickedHome();

private:
	UUIManager* uiManager;
	bool isBinding;
};
