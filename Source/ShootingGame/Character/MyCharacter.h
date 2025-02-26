// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ABaseWeapon;

struct FInputActionValue;

UCLASS()
class SHOOTINGGAME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	UCameraComponent* CameraComp;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartCrouch(const FInputActionValue& value);
	UFUNCTION()
	void StopCrouch(const FInputActionValue& value);
	UFUNCTION(BlueprintCallable, Category = "Character")
	bool GetIsCrouching() const;

	UFUNCTION()
	void TakeDamage(float DamageAmount);
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformMeleeAttack();
	UFUNCTION()
	void StartHealing();

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Movement")
	bool bIsCrouching;



private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
	float CrouchSpeed;
	float CrouchSpeedMultiplier;
	float DefaultFOV;
	float ZoomedFOV = 60.0f;
	float ZoomInterpSpeed = 10.0f;


	UPROPERTY(EditAnywhere, Category = "Health")
	float Health = 100.0f;
	float MeleeDamage = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ABaseWeapon> DefaultWeaponClass;

	UPROPERTY()
	ABaseWeapon* EquippedWeapon;

	FTimerHandle HealingTimerHandle;
	UFUNCTION()
	void CompleteHealing();

	UPROPERTY()
	bool bIsZooming = false;

	UFUNCTION()
	void StartZoom();
	UFUNCTION()
	void StopZoom();
};
