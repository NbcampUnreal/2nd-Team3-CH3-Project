#include "Core/DefaultGameConfig.h"
#include "Misc/ConfigCacheIni.h"

FString UDefaultGameConfig::GetGameVersion()
{
    FString Version;
    if (GConfig)
    {
        GConfig->GetString(TEXT("System"), TEXT("Version"), Version, GGameIni);
    }
    return Version;
}

bool UDefaultGameConfig::GetShowLog()
{
    bool ShowLog;
    if (GConfig)
    {
        GConfig->GetBool(TEXT("System"), TEXT("ShowLog"), ShowLog, GGameIni);
    }
    return ShowLog;
}
