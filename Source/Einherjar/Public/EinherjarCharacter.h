#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "EinherjarCharacter.generated.h"

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

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UStaticMesh* WeaponMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector WeaponLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FRotator WeaponRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector WeaponScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bUsesShield = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UStaticMesh* ShieldMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector ShieldLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FRotator ShieldRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FVector ShieldScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bTwoHanded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Stats")
	float DamageMultiplier = 1.0f;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_ToggleWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon Slots")
	UInputAction* IA_WeaponSlot1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon Slots")
	UInputAction* IA_WeaponSlot2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon Slots")
	UInputAction* IA_WeaponSlot3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon Slots")
	UInputAction* IA_WeaponSlot4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon Slots")
	UInputAction* IA_WeaponSlot5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon Slots")
	UInputAction* IA_WeaponSlot6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon Slots")
	UInputAction* IA_WeaponSlot7;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon Slots")
	UInputAction* IA_WeaponSlot8;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon Slots")
	UInputAction* IA_WeaponSlot9;

	// ============================================================
	// WEAPON / LOADOUT SYSTEM
	// ============================================================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	UDataTable* WeaponDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName DefaultWeaponRow = "Sword";

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
	float KickStunDuration = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Tracing")
	float KickRange = 100.0f;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	bool bAutoRespawn = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float RespawnDelay = 3.0f;

	// ============================================================
	// STAMINA SYSTEM
	// ============================================================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float MaxStamina = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaRegenRate = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaRegenDelay = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float LightAttackStaminaCost = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float HeavyAttackStaminaCost = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float DefenseStaminaCost = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float KickStaminaCost = 20.0f;

	// ============================================================
	// AI SYSTEM
	// ============================================================
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool bIsAIControlled = false;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AIDecisionIntervalMin = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AIDecisionIntervalMax = 3.0f;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AIAttackChance = 0.6f;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AIBlockDuration = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AIRotationSpeed = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AIEngageRange = 400.0f;

	// ============================================================
	// HEALTH & STAMINA — BLUEPRINT-CALLABLE FUNCTIONS
	// ============================================================

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Respawn();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat|Defense")
	bool CanBlockAttack(ECombatDirection IncomingDirection) const;

	UFUNCTION(BlueprintCallable, Category = "Combat|Defense")
	void SetForceDefense(ECombatDirection DefenseDirection);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool ConsumeStamina(float Amount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stamina")
	float GetStaminaPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyKnockback(FVector Direction, float Force);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ApplyStun(float Duration);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformAttackTrace();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipWeapon(FName WeaponRowName);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SelectWeaponSlot(int32 SlotIndex);

	UFUNCTION(BlueprintPure, Category = "Weapon")
	int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }

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
	void OnMouseAttackStarted();
	void OnMouseAttackReleased();
	void OnMouseDefenseStarted();
	void OnMouseDefenseReleased();	
	void OnToggleWeapon();
	void OnWeaponSlot1();
	void OnWeaponSlot2();
	void OnWeaponSlot3();
	void OnWeaponSlot4();
	void OnWeaponSlot5();
	void OnWeaponSlot6();
	void OnWeaponSlot7();
	void OnWeaponSlot8();
	void OnWeaponSlot9();
	void ResetCombatState();
	void EndRecovery();
	void EndStun();
	void ClearPendingDirections();
	void ExecuteAttack(ECombatDirection Direction, bool bHeavy);
	void StopDefending();	
	void AIMakeDecision();	
	void AIScheduleNextDecision();
	void AIStopBlocking();

private:
	FTimerHandle CombatStateResetTimerHandle;
	FTimerHandle AttackHitTimerHandle;
	FTimerHandle RespawnTimerHandle;
	FTimerHandle StunTimerHandle;
	FTimerHandle AIDecisionTimerHandle;
	FTimerHandle AIBlockTimerHandle;

	bool bIsHoldingKeyboardDefense = false;
	int32 CurrentWeaponIndex = 0;
	float LastStaminaUseTime = 0.0f;

	UPROPERTY()
	class UStaticMeshComponent* CachedWeaponMesh = nullptr;

	UPROPERTY()
	class UStaticMeshComponent* CachedShieldMesh = nullptr;

	// Mouse tracking state
	bool bIsTrackingAttack = false;
	bool bIsTrackingDefense = false;
	bool bIsHoldingMouseDefense = false;
	FVector2D AccumulatedMouseDelta = FVector2D::ZeroVector;
	float MouseAttackStartTime = 0.0f;

	bool bWeaponDrawn = true;

};