#pragma once

#include "CoreMinimal.h"
#include "ItemProperty.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Weapon          UMETA(DisplayName = "Weapon", ToolTip = "무기"),
    Consumable      UMETA(DisplayName = "Consumable", ToolTip = "소모품"),
    Misc            UMETA(DisplayName = "Miscellaneous", ToolTip = "잡다한")
};

UENUM(BlueprintType)
enum class EItemName : uint8
{
    GreenHerb       UMETA(DisplayName = "GreenHerb", ToolTip = "체력을 약간 회복시키는 녹색 약초"),
    RedHerb         UMETA(DisplayName = "RedHerb", ToolTip = "조합하는 다른 약초들의 효과를 증대시키는 빨간 약초. 단독으로 사용할 수 없다."),
    YellowHerb      UMETA(DisplayName = "YellowHerb", ToolTip = "체력의 최대치를 상승시키는 노란 약초. 어떤 약초를 조합하는가에 따라 상승치가 달라진다. 단독으로 사용할 수 없다."),
    Ammo            UMETA(DisplayName = "Ammo", ToolTip = "평범한 탄약"),
    Ammo2           UMETA(DisplayName = "Ammo2", ToolTip = "고급 탄약"),
    Ammo3           UMETA(DisplayName = "Ammo3", ToolTip = "미친 탄약"),
};
