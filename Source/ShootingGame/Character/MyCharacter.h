// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ABaseWeapon;
class UChildActorComponent;

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

	// ====================== ����, �κ��丮, ������ ��ȣ�ۿ�  ===========================
	bool bIsShowInventory;
	UFUNCTION(BlueprintCallable)
	void OnInputInventoryKey();

	bool bIsShowPauseUI;
	UFUNCTION(BlueprintCallable)
	void OnInputESCKey();

	void TryPickUp();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ======================  ĳ���� �̵�����  ===========================
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

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Movement")
	bool bIsCrouching;
	UFUNCTION()
	void StartCrouch(const FInputActionValue& value);
	UFUNCTION()
	void StopCrouch(const FInputActionValue& value);
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsCrouching() const;

	// ======================  ĳ���� ����, ��  ===========================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* WeaponSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ABaseWeapon* EquippedWeapon;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformMeleeAttack();

	// ======================  ĳ���� ���� ���� �� ������ ���  ===========================
	void TryEquipMainWeapon();
	void TryEquipSubWeapon();
	void TryEquipMeleeWeapon();
	void TryEquipThrowableWeapon();
	UFUNCTION()
	void TryUseHealingItem();

	// ====================== ĳ���� ����� ó��  ===========================
	UFUNCTION()
	void CharacterTakeDamage(float DamageAmount);
	UFUNCTION()
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	void OnPlayerDeath();

	// ====================== ȸ�� ����  ===========================
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* Turn90Anim;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float YawOffset;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float PreviousYaw;

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
	float CrouchSpeed;
	float CrouchSpeedMultiplier;
	float DefaultFOV;
	float ZoomedFOV = 60.0f;
	float ZoomInterpSpeed = 10.0f;
	FVector FirstPersonCameraOffset = FVector(0.f, 0.f, 60.f);
	FVector SavedCameraLocation;
	FRotator SavedCameraRotation;


	UPROPERTY(EditAnywhere, Category = "Health")
	float Health = 100.0f;
	float MeleeDamage = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ABaseWeapon> DefaultWeaponClass;


	FTimerHandle HealingTimerHandle;
	UFUNCTION()
	void CompleteHealing();

	UPROPERTY()
	bool bIsZooming = false;

	bool bIsTurning = false;
	bool bIsFirstPerson = false;
	bool bInterpToThirdPerson = false;

	UFUNCTION()
	void StartZoom();
	UFUNCTION()
	void StopZoom();

	void ToggleFirstPerson();
};
