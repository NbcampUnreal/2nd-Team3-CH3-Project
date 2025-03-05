// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemProperty.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


class USphereComponent;
class UWidgetComponent;

UCLASS()
class SHOOTINGGAME_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Component")
	UUserWidget* Widget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	USphereComponent* Collision;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemProperty")
	EItemName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemProperty")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|ItemProperty")
	UTexture2D* ItemImage;
	

protected:
	virtual void BeginPlay() override;	

	//충돌처리 함수
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UWidgetComponent* GetHud();
};
