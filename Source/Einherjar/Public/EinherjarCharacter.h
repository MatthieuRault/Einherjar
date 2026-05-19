// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EinherjarCharacter.generated.h"

// Forward declaration to avoid including InputAction.h in the header
class UInputAction;
class UInputComponent;

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
	// These variables are assigned from the derived Blueprint.
	// ============================================================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_Overhead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_Stab;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_LeftSlash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Input")
	UInputAction* IA_RightSlash;

protected:
	// ============================================================
	// COMBAT — HANDLERS
	// Functions called when the player presses input keys.
	// ============================================================

	void OnOverhead();
	void OnStab();
	void OnLeftSlash();
	void OnRightSlash();
};