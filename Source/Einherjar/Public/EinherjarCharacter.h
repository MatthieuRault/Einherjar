// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EinherjarCharacter.generated.h"

// Forward declaration to avoid including InputAction.h in the header
class UInputAction;
class UInputComponent;
// Combat enum
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
	Stunned      UMETA(DisplayName = "Stunned")
};

UCLASS()
class EINHERJAR_API AEinherjarCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEinherjarCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind input functionality
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

	// ============================================================
	// COMBAT — STATE
	// ============================================================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
	ECombatAction CurrentCombatAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|State")
	ECombatDirection CurrentCombatDirection;

protected:
	// ============================================================
	// COMBAT — HANDLERS
	// Functions called when the player presses input keys.
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
};