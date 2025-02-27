#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DefaultGameConfig.generated.h"


UCLASS()
class SHOOTINGGAME_API UDefaultGameConfig : public UObject
{
	GENERATED_BODY()

public:
	static FString GetGameVersion();
	static bool GetShowLog();

	
};
