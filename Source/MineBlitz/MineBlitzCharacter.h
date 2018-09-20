// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MineBlitzCharacter.generated.h"

UCLASS()
class MINEBLITZ_API AMineBlitzCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
public:
	AMineBlitzCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/**Check if were running*/
	bool bIsRunning;

	/**Check if were mining*/
	bool bIsMining;

	/**Check if were allowed to mine*/
	bool bCanMine;

	/** Sound to play each time we mine */

protected:

	//Input Portion
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void ToggleRun();
	void Mine();
	void ResetMine();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	float CurrentStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	float StaminaRegenRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	float CurrentMoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	float WeightMultiplier;

	UPROPERTY(EditDefaultsOnly)
	float MiningMontageTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	int GoldResource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	int CopperResource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	int IronResource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterStats")
	int PickAxePower;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mining")
	float MineCost;

	FTimerHandle MiningDelayHandle;

	FTimerHandle ObjectDestroyHandle;

	void CheckObject();

	void ResetMiningTime();

	void RegenerateStamina(float DeltaTime);

	/**Pickaxe Things*/
	UPROPERTY(BluePrintReadWrite)
	bool ObjectCanHit;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MyPickAxe;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* PickAxeCapsuleComp;

	UFUNCTION()
	void CollisionEnter(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(BluePrintReadWrite)
	int Score;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**Animation*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* MiningMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* ImpactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining")
	class USoundCue* OreHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining")
	class USoundCue* GruntSound;

private:
	class AMinableObject* MinableObject;

public:
	void Tick(float DeltaTime) override;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return Camera; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool bIsCharRunning() const { return bIsRunning; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool bIsCharMining() const { return bIsMining; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentStamina() const { return CurrentStamina; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetMyCopperResources() const { return CopperResource; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetMyIronResources() const { return IronResource; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetMyGoldResources() const { return GoldResource; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMineCost() const { return MineCost; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetScore() const { return Score; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetPickAxePower() const { return PickAxePower; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetSpeed() const { return CurrentMoveSpeed; }

	UFUNCTION(BlueprintCallable)
	void UpgradePickAxePower();

	UFUNCTION(BlueprintCallable)
	void SetMoveSpeed();

	UFUNCTION(BlueprintCallable)
	void SetStamina();

	UFUNCTION(BlueprintCallable)
	void CashOutOres(int Round);

	UFUNCTION(BlueprintCallable)
	void ResetBools();

	UFUNCTION(BlueprintCallable)
	void ResetStamina();
};