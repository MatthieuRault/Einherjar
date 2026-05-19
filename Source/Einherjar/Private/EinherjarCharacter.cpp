// Fill out your copyright notice in the Description page of Project Settings.

#include "EinherjarCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

// Sets default values
AEinherjarCharacter::AEinherjarCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentCombatAction = ECombatAction::None;
	CurrentCombatDirection = ECombatDirection::None;
}

// Called when the game starts or when spawned
void AEinherjarCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Einherjar Character spawned!"));
}

// Called every frame
void AEinherjarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind input functionality
void AEinherjarCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Cast the InputComponent to EnhancedInputComponent (modern input system)
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Attacks
		if (IA_Overhead)    EnhancedInput->BindAction(IA_Overhead, ETriggerEvent::Started, this, &AEinherjarCharacter::OnOverhead);
		if (IA_Stab)        EnhancedInput->BindAction(IA_Stab, ETriggerEvent::Started, this, &AEinherjarCharacter::OnStab);
		if (IA_LeftSlash)   EnhancedInput->BindAction(IA_LeftSlash, ETriggerEvent::Started, this, &AEinherjarCharacter::OnLeftSlash);
		if (IA_RightSlash)  EnhancedInput->BindAction(IA_RightSlash, ETriggerEvent::Started, this, &AEinherjarCharacter::OnRightSlash);
		if (IA_Kick)          EnhancedInput->BindAction(IA_Kick, ETriggerEvent::Started, this, &AEinherjarCharacter::OnKick);
		if (IA_AttackCancel)  EnhancedInput->BindAction(IA_AttackCancel, ETriggerEvent::Started, this, &AEinherjarCharacter::OnAttackCancel);

		// Defenses
		if (IA_DefenseLeft)    EnhancedInput->BindAction(IA_DefenseLeft, ETriggerEvent::Started, this, &AEinherjarCharacter::OnDefenseLeft);
		if (IA_DefenseCenter)  EnhancedInput->BindAction(IA_DefenseCenter, ETriggerEvent::Started, this, &AEinherjarCharacter::OnDefenseCenter);
		if (IA_DefenseRight)   EnhancedInput->BindAction(IA_DefenseRight, ETriggerEvent::Started, this, &AEinherjarCharacter::OnDefenseRight);

	}
}

// ============================================================
// COMBAT — HANDLERS
// ============================================================

// Attacks

void AEinherjarCharacter::OnOverhead()
{
	CurrentCombatAction = ECombatAction::Attacking;
	CurrentCombatDirection = ECombatDirection::Up;
	UE_LOG(LogTemp, Warning, TEXT("Attack: Overhead | State: Attacking/Up"));;
}

void AEinherjarCharacter::OnStab()
{
	CurrentCombatAction = ECombatAction::Attacking;
	CurrentCombatDirection = ECombatDirection::Down;
	UE_LOG(LogTemp, Warning, TEXT("Attack: Stab | State: Attacking/Down"));
}

void AEinherjarCharacter::OnLeftSlash()
{
	CurrentCombatAction = ECombatAction::Attacking;
	CurrentCombatDirection = ECombatDirection::Left;
	UE_LOG(LogTemp, Warning, TEXT("Attack: Left Slash | State: Attacking/Left"));
}

void AEinherjarCharacter::OnRightSlash()
{
	CurrentCombatAction = ECombatAction::Attacking;
	CurrentCombatDirection = ECombatDirection::Right;
	UE_LOG(LogTemp, Warning, TEXT("Attack: Right Slash | State: Attacking/Right"));
}

void AEinherjarCharacter::OnKick()
{
	UE_LOG(LogTemp, Warning, TEXT("Kick! (l'etat ne change pas)"));
}

void AEinherjarCharacter::OnAttackCancel()
{
	CurrentCombatAction = ECombatAction::None;
	CurrentCombatDirection = ECombatDirection::None;;
	UE_LOG(LogTemp, Warning, TEXT("Attack Cancel | State: None"));
}

// Defense

void AEinherjarCharacter::OnDefenseLeft()
{
	CurrentCombatAction = ECombatAction::Defending;
	CurrentCombatDirection = ECombatDirection::Left;
	UE_LOG(LogTemp, Warning, TEXT("Defense: Left | State: Defending/Left"));
}

void AEinherjarCharacter::OnDefenseCenter()
{
	CurrentCombatAction = ECombatAction::Defending;
	CurrentCombatDirection = ECombatDirection::Center;
	UE_LOG(LogTemp, Warning, TEXT("Defense: Center | State: Defending/Center"));
}

void AEinherjarCharacter::OnDefenseRight()
{
	CurrentCombatAction = ECombatAction::Defending;
	CurrentCombatDirection = ECombatDirection::Right;
	UE_LOG(LogTemp, Warning, TEXT("Defense: Right | State: Defending/Right"));
}