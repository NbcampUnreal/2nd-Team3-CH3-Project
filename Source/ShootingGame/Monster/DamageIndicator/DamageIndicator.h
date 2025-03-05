#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageIndicator.generated.h"

UCLASS()
class SHOOTINGGAME_API ADamageIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	ADamageIndicator();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWidgetComponent* WidgetComponent;

	void SetDamage(float Damage);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	//TSubclassOf<class UWidgetDamageIndicator> DamageWidgetClass;

private:
	UPROPERTY()
	APlayerCameraManager* PlayerCameraManager;

	FTimerHandle DestroyTimerHandle;

	void DestroySelf();

	//void OnFadeOutFinished();
};
