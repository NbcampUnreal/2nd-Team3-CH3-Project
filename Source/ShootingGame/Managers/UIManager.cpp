#include "UIManager.h"

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("Init UIManager"));

}

void UUIManager::ShowIntroUI()
{
	UE_LOG(LogTemp, Warning, TEXT("Show Intro UI!!"));
}

void UUIManager::ShowMainUI()
{
	UE_LOG(LogTemp, Warning, TEXT("Show Main UI!!"));
}

void UUIManager::ShowSettingUI()
{
	UE_LOG(LogTemp, Warning, TEXT("Show Setting UI!!"));
}

void UUIManager::ShowInGameUI()
{
	UE_LOG(LogTemp, Warning, TEXT("Show InGame UI!!"));
}
