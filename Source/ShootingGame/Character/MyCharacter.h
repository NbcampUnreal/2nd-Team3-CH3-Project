// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ABaseWeapon;
class AMainWeapon;
class ASubWeapon;
class AMeleeWeapon;
class AThrowableWeapon;
class AMagazine;
class UChildActorComponent;
class UHexboundGameInstance;
class UUIManager;

struct FInputActionValue;

UCLASS()
class SHOOTINGGAME_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();
	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	UCameraComponent* CameraComp;

	//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UHexPlayerHUD> HUDWidgetClass;

	UPROPERTY()
	UHexPlayerHUD* HUDWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* MainWeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* SubWeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* MeleeWeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* ThrowableWeaponActor;


	// 무기 변경 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* WeaponSwapAnimation;

	// 무기 변경 애니메이션 실행 함수
	void PlayWeaponSwapAnimation(TFunction<void()> OnAnimationEnd);

	// 캐릭터의 애니메이션 인스턴스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimInstance* CharacterAnimInstance;

	// 무기 변경 중 상태 플래그
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsSwappingWeapon = false;

	// 무기 교체 완료 콜백 함수
	void FinishWeaponSwap();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
	void SetWeaponSwapState(bool bIsSwapping);

	// 타이머 핸들
	FTimerHandle SwapWeaponTimerHandle;

	// ====================== 설정, 인벤토리, 아이템 상호작용  ===========================
	bool bIsShowInventory;
	UFUNCTION(BlueprintCallable)
	void OnInputInventoryKey();

	bool bIsShowPauseUI;
	UFUNCTION(BlueprintCallable)
	void OnInputESCKey();

	void TryPickUp();


	// 재장전 애니메이션 시퀀스 (Dynamic Montage로 재생할 애니메이션)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* ReloadSequence;

	// 재장전 상태 bool (AnimBP와 연동할 변수)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	bool bIsReloading = false;

	// 재장전 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TryReload();

	// 재장전 상태 전환 함수
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartReload();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndReload();

	UFUNCTION(BlueprintPure, Category = "Weapon")
	ABaseWeapon* GetCurrentWeapon() const;


protected:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ======================  캐릭터 이동관련  ===========================
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

	// ======================  캐릭터 공격, 줌  ===========================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* WeaponSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* MainWeaponMagz;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UChildActorComponent* SubWeaponMagz;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	ABaseWeapon* EquippedWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AMagazine* MainWeaponMagActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AMagazine* SubWeaponMagActor;
	
	UUIManager* UIManagerForInitUI;
	UTexture2D* InitUIImage;
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformMeleeAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* HitReactionSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* DeathSequence;

	//void TryReload();
	void AttachParts();
	void TryAddAmmo();

	void DestroyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AParts> Parts;

	AParts* InstanceParts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	bool IsMenuLevel = false;
	// ======================  캐릭터 무기 장착 및 아이템 사용  ===========================
	void TryEquipMainWeapon();
	void TryEquipSubWeapon();
	void TryEquipMeleeWeapon();
	void TryEquipThrowableWeapon();
	UFUNCTION()
	void TryUseHealingItem();

	// ====================== 캐릭터 대미지 처리  ===========================
	UFUNCTION()
	void CharacterTakeDamage(float DamageAmount);
	UFUNCTION()
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	void OnPlayerDeath();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsDead = false;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsDead() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsHitReacting = false;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetIsHitReacting() const;
	UFUNCTION()
	void InitializingUI();

	// ====================== 회전 관련  ===========================

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


	float MeleeDamage = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ABaseWeapon> DefaultWeaponClass;

	UHexboundGameInstance* OurGameInstance;
	FTimerHandle HealingTimerHandle;
	UFUNCTION()
	void CompleteHealing();

	UPROPERTY()
	bool bIsZooming = false;

	bool bIsTurning = false;
	bool bIsFirstPerson = false;
	bool bInterpToThirdPerson = false;
	bool bIsSubWeaponMagExist = false;
	bool bIsMainWeaponMagExist = false;

	UFUNCTION()
	void StartZoom();
	UFUNCTION()
	void StopZoom();

	void ToggleFirstPerson();


	void LogFireAmmoState(class AFirearm* fireWeapon);

	//재장전
	FTimerHandle ReloadTimerHandle;
	FTimerHandle DeathTimerHandle;

	//피격
	FTimerHandle HitResetTimerHandle;
	void ResetHitState();
};
