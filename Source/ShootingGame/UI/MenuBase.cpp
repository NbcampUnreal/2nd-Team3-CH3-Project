#include "UI/MenuBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Core/DefaultGameConfig.h"


void UMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

    if (VersionText)
    {
        VersionText->SetText(FText::FromString(UDefaultGameConfig::GetGameVersion()));
    }
}
