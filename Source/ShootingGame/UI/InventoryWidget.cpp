#include "UI/InventoryWidget.h"
#include "Components/Button.h"
#include "Managers/UIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Default/DefaultCharacter.h"
#include "Character/MyCharacter.h"

void UInventoryWidget::NativeConstruct()
{
	if (!isBinding)
	{
		uiManager = GetGameInstance()->GetSubsystem<UUIManager>();

		if (ButtonClose)
		{
			ButtonClose->OnClicked.AddDynamic(this, &UInventoryWidget::OnClickedClose);
		}

		isBinding = true;
	}
}

void UInventoryWidget::OnClickedClose()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("OnClicked Closed Button")));

	if (uiManager)
	{
		ACharacter* playedCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (ADefaultCharacter* defaultCharacter = Cast<ADefaultCharacter>(playedCharacter))
		{
			defaultCharacter->OnInputInventoryKey();
		}
		else if (AMyCharacter* myCharacter = Cast<AMyCharacter>(playedCharacter))
		{
			myCharacter->OnInputInventoryKey();
		}
	}
}
