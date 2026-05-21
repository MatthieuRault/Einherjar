// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EinherjarCharacter.generated.h"

// Forward declaration
class UInputAction;
class UInputComponent;

// Combat enums
UENUM(BlueprintType)
enum class ECombatDirection : uint8
{
	None      UMETA(DisplayName = "None"),
	Up        UMETA(DisplayName = "Up"),
	Down      UMETA(DisplayName = "Down"),
	Left      UMETA(DisplayName = "Left"),
	Right     UMETA(DisplayName = "Right"),
	Center    UMETA(DisplayName = "Center")
};

UENUM(BlueprintType)
enum class ECombatAction : uint8
{
	None         UMETA(DisplayName = "None"),
	Attacking    UMETA(DisplayName = "Attacking"),
	Defending    UMETA(DisplayName = "Defending"),
	Recovering   UMETA(DisplayName = "Recovering"),
	Stunned      UMETA(DisplayName = "Stunned")
};

UCLASS()
class EINHERJAR_API AEinherjarCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEinherjarCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ============================================================
	// COMBAT — INPUT ACTIONS
	// ============================================================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_Overhead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_Stab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_LeftSlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_RightSlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_DefenseLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_DefenseCenter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_DefenseRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_Kick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_AttackCancel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_MouseAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_MouseDefense;

	// ============================================================
	// COMBAT — STATE
	// ============================================================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
	ECombatAction CurrentCombatAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
	ECombatDirection CurrentCombatDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|State")
	float AttackDuration = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|State")
	float RecoveryDuration = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Tracing")
	float HeavyAttackThreshold = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Tracing")
	float HeavyAttackDamage = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
	bool bWasLastAttackHeavy = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
	ECombatDirection PendingAttackDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
	ECombatDirection PendingDefenseDirection;

	// ============================================================
	// COMBAT — ATTACK PARAMETERS
	// ============================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Tracing")
	float AttackHitDelay = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Tracing")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Tracing")
	float AttackRadius = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Tracing")
	float AttackDamage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Tracing")
	bool bDrawDebugTrace = true;

	// ============================================================
	// HEALTH SYSTEM
	// ============================================================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;

	// ============================================================
	// HEALTH — BLUEPRINT-CALLABLE FUNCTIONS
	// ============================================================

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|Defense")
	bool CanBlockAttack(ECombatDirection IncomingDirection) const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Defense")
	void SetForceDefense(ECombatDirection DefenseDirection);

protected:
	// ============================================================
	// COMBAT — HANDLERS
	// ============================================================

	void OnOverhead();
	void OnStab();
	void OnLeftSlash();
	void OnRightSlash();
	void OnDefenseLeft();
	void OnDefenseCenter();
	void OnDefenseRight();
	void OnKick();
	void OnAttackCancel();
	void ResetCombatState();
	void EndRecovery();
	void OnMouseAttackStarted();
	void OnMouseAttackReleased();
	void OnMouseDefenseStarted();
	void OnMouseDefenseReleased();
	void PerformAttackTrace();
	void ClearPendingDirections();
	void ExecuteAttack(ECombatDirection Direction, bool bHeavy);

private:
	FTimerHandle CombatStateResetTimerHandle;
	FTimerHandle AttackHitTimerHandle;

	// Mouse tracking state
	bool bIsTrackingAttack = false;
	bool bIsTrackingDefense = false;
	FVector2D AccumulatedMouseDelta = FVector2D::ZeroVector;
	float MouseAttackStartTime = 0.0f;
};