#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

class UImage;

UCLASS()
class SHOOTINGGAME_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* BackGroundImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* VersionText;
	
protected:
	virtual void NativeConstruct() override;

};
