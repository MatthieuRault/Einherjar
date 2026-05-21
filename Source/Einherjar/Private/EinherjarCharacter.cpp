// Fill out your copyright notice in the Description page of Project Settings.

#include "EinherjarCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"

// Sets default values
AEinherjarCharacter::AEinherjarCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CurrentCombatAction = ECombatAction::None;
	CurrentCombatDirection = ECombatDirection::None;
	PendingAttackDirection = ECombatDirection::None;
	PendingDefenseDirection = ECombatDirection::None;

	// Health initialization
	CurrentHealth = MaxHealth;
	bIsDead = false;

	// Stamina initialization
	CurrentStamina = MaxStamina;
}

void AEinherjarCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("%s spawned with %.1f HP"), *GetName(), CurrentHealth);
}

void AEinherjarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTrackingAttack || bIsTrackingDefense)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			float DeltaX = 0.0f;
			float DeltaY = 0.0f;
			PC->GetInputMouseDelta(DeltaX, DeltaY);

			AccumulatedMouseDelta.X += DeltaX;
			AccumulatedMouseDelta.Y += DeltaY;
		}
	}

	if (!bIsDead && CurrentStamina < MaxStamina)
	{
		const float TimeSinceLastUse = GetWorld()->GetTimeSeconds() - LastStaminaUseTime;
		if (TimeSinceLastUse >= StaminaRegenDelay)
		{
			CurrentStamina = FMath::Clamp(
				CurrentStamina + StaminaRegenRate * DeltaTime,
				0.0f,
				MaxStamina
			);
		}
	}
}

void AEinherjarCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IA_Overhead)    EnhancedInput->BindAction(IA_Overhead, ETriggerEvent::Started, this, &AEinherjarCharacter::OnOverhead);
		if (IA_Stab)        EnhancedInput->BindAction(IA_Stab, ETriggerEvent::Started, this, &AEinherjarCharacter::OnStab);
		if (IA_LeftSlash)   EnhancedInput->BindAction(IA_LeftSlash, ETriggerEvent::Started, this, &AEinherjarCharacter::OnLeftSlash);
		if (IA_RightSlash)  EnhancedInput->BindAction(IA_RightSlash, ETriggerEvent::Started, this, &AEinherjarCharacter::OnRightSlash);
		if (IA_Kick)          EnhancedInput->BindAction(IA_Kick, ETriggerEvent::Started, this, &AEinherjarCharacter::OnKick);
		if (IA_AttackCancel)  EnhancedInput->BindAction(IA_AttackCancel, ETriggerEvent::Started, this, &AEinherjarCharacter::OnAttackCancel);

		if (IA_DefenseLeft)    EnhancedInput->BindAction(IA_DefenseLeft, ETriggerEvent::Started, this, &AEinherjarCharacter::OnDefenseLeft);
		if (IA_DefenseCenter)  EnhancedInput->BindAction(IA_DefenseCenter, ETriggerEvent::Started, this, &AEinherjarCharacter::OnDefenseCenter);
		if (IA_DefenseRight)   EnhancedInput->BindAction(IA_DefenseRight, ETriggerEvent::Started, this, &AEinherjarCharacter::OnDefenseRight);

		if (IA_MouseAttack)
		{
			EnhancedInput->BindAction(IA_MouseAttack, ETriggerEvent::Started, this, &AEinherjarCharacter::OnMouseAttackStarted);
			EnhancedInput->BindAction(IA_MouseAttack, ETriggerEvent::Completed, this, &AEinherjarCharacter::OnMouseAttackReleased);
		}
		if (IA_MouseDefense)
		{
			EnhancedInput->BindAction(IA_MouseDefense, ETriggerEvent::Started, this, &AEinherjarCharacter::OnMouseDefenseStarted);
			EnhancedInput->BindAction(IA_MouseDefense, ETriggerEvent::Completed, this, &AEinherjarCharacter::OnMouseDefenseReleased);
		}
	}
}

// ============================================================
// ATTACKS
// ============================================================

void AEinherjarCharacter::OnOverhead()
{
	PendingAttackDirection = ECombatDirection::Up;
	UE_LOG(LogTemp, Warning, TEXT(">>> Pending attack: UP (Overhead)"));
}

void AEinherjarCharacter::OnStab()
{
	PendingAttackDirection = ECombatDirection::Down;
	UE_LOG(LogTemp, Warning, TEXT(">>> Pending attack: DOWN (Stab)"));
}

void AEinherjarCharacter::OnLeftSlash()
{
	PendingAttackDirection = ECombatDirection::Left;
	UE_LOG(LogTemp, Warning, TEXT(">>> Pending attack: LEFT (Left Slash)"));
}

void AEinherjarCharacter::OnRightSlash()
{
	PendingAttackDirection = ECombatDirection::Right;
	UE_LOG(LogTemp, Warning, TEXT(">>> Pending attack: RIGHT (Right Slash)"));
}

void AEinherjarCharacter::OnKick()
{
	if (!ConsumeStamina(KickStaminaCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("Kick failed: not enough stamina"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Kick! | Stamina: %.1f"), CurrentStamina);
}

void AEinherjarCharacter::OnAttackCancel()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Cancel | clearing all state"));
	GetWorldTimerManager().ClearTimer(CombatStateResetTimerHandle);
	GetWorldTimerManager().ClearTimer(AttackHitTimerHandle);

	CurrentCombatAction = ECombatAction::None;
	CurrentCombatDirection = ECombatDirection::None;

	ClearPendingDirections();
}

// ============================================================
// DEFENSES
// ============================================================

void AEinherjarCharacter::OnDefenseLeft()
{
	PendingDefenseDirection = ECombatDirection::Left;
	UE_LOG(LogTemp, Warning, TEXT(">>> Pending defense: LEFT"));
}

void AEinherjarCharacter::OnDefenseCenter()
{
	PendingDefenseDirection = ECombatDirection::Center;
	UE_LOG(LogTemp, Warning, TEXT(">>> Pending defense: CENTER"));
}

void AEinherjarCharacter::OnDefenseRight()
{
	PendingDefenseDirection = ECombatDirection::Right;
	UE_LOG(LogTemp, Warning, TEXT(">>> Pending defense: RIGHT"));
}

// ============================================================
// HELPERS
// ============================================================

void AEinherjarCharacter::ResetCombatState()
{
	if (CurrentCombatAction == ECombatAction::Attacking)
	{
		CurrentCombatAction = ECombatAction::Recovering;
		CurrentCombatDirection = ECombatDirection::None;
		UE_LOG(LogTemp, Warning, TEXT("Combat state: Recovering for %.2fs"), RecoveryDuration);

		GetWorldTimerManager().SetTimer(
			CombatStateResetTimerHandle,
			this,
			&AEinherjarCharacter::EndRecovery,
			RecoveryDuration,
			false
		);
	}
	else
	{
		CurrentCombatAction = ECombatAction::None;
		CurrentCombatDirection = ECombatDirection::None;
		UE_LOG(LogTemp, Warning, TEXT("Combat state reset to None"));
	}
}

void AEinherjarCharacter::EndRecovery()
{
	CurrentCombatAction = ECombatAction::None;
	CurrentCombatDirection = ECombatDirection::None;
	UE_LOG(LogTemp, Warning, TEXT("Recovery done, ready to attack again"));
}

void AEinherjarCharacter::ClearPendingDirections()
{
	PendingAttackDirection = ECombatDirection::None;
	PendingDefenseDirection = ECombatDirection::None;
}

// ============================================================
// MOUSE-BASED COMBAT
// ============================================================

void AEinherjarCharacter::OnMouseAttackStarted()
{
	bIsTrackingAttack = true;
	AccumulatedMouseDelta = FVector2D::ZeroVector;
	MouseAttackStartTime = GetWorld()->GetTimeSeconds();
}

void AEinherjarCharacter::OnMouseAttackReleased()
{
	if (!bIsTrackingAttack) return;
	bIsTrackingAttack = false;

	const float HoldDuration = GetWorld()->GetTimeSeconds() - MouseAttackStartTime;
	const bool bIsHeavy = HoldDuration >= HeavyAttackThreshold;

	ECombatDirection AttackDirection = ECombatDirection::None;

	if (PendingAttackDirection != ECombatDirection::None)
	{
		AttackDirection = PendingAttackDirection;
		UE_LOG(LogTemp, Warning, TEXT("Mouse attack: using KEYBOARD direction (%s) | %s"),
			*UEnum::GetValueAsString(AttackDirection),
			bIsHeavy ? TEXT("HEAVY") : TEXT("LIGHT"));
	}
	else
	{
		const float AbsX = FMath::Abs(AccumulatedMouseDelta.X);
		const float AbsY = FMath::Abs(AccumulatedMouseDelta.Y);
		const float MinThreshold = 1.0f;

		if (AbsX < MinThreshold && AbsY < MinThreshold)
		{
			UE_LOG(LogTemp, Verbose, TEXT("Mouse attack released without direction, ignored"));
			return;
		}

		if (AbsY > AbsX)
		{
			AttackDirection = (AccumulatedMouseDelta.Y < 0) ? ECombatDirection::Down : ECombatDirection::Up;
		}
		else
		{
			AttackDirection = (AccumulatedMouseDelta.X < 0) ? ECombatDirection::Left : ECombatDirection::Right;
		}

		UE_LOG(LogTemp, Warning, TEXT("Mouse attack: using MOUSE direction (%s) | %s"),
			*UEnum::GetValueAsString(AttackDirection),
			bIsHeavy ? TEXT("HEAVY") : TEXT("LIGHT"));
	}

	ExecuteAttack(AttackDirection, bIsHeavy);
}

void AEinherjarCharacter::OnMouseDefenseStarted()
{
	bIsTrackingDefense = true;
	AccumulatedMouseDelta = FVector2D::ZeroVector;
}

void AEinherjarCharacter::OnMouseDefenseReleased()
{
	if (!bIsTrackingDefense) return;
	bIsTrackingDefense = false;

	ECombatDirection DefenseDirection = ECombatDirection::None;

	if (PendingDefenseDirection != ECombatDirection::None)
	{
		DefenseDirection = PendingDefenseDirection;
		UE_LOG(LogTemp, Warning, TEXT("Mouse defense: using KEYBOARD direction (%s)"),
			*UEnum::GetValueAsString(DefenseDirection));
	}
	else
	{
		const float AbsX = FMath::Abs(AccumulatedMouseDelta.X);
		const float CenterThreshold = 2.0f;

		if (AbsX < CenterThreshold)
		{
			DefenseDirection = ECombatDirection::Center;
		}
		else if (AccumulatedMouseDelta.X < 0)
		{
			DefenseDirection = ECombatDirection::Left;
		}
		else
		{
			DefenseDirection = ECombatDirection::Right;
		}

		UE_LOG(LogTemp, Warning, TEXT("Mouse defense: using MOUSE direction (%s)"),
			*UEnum::GetValueAsString(DefenseDirection));
	}

	if (CurrentCombatAction != ECombatAction::None) return;

	if (!ConsumeStamina(DefenseStaminaCost))
	{
		UE_LOG(LogTemp, Warning, TEXT("Defense failed: not enough stamina"));
		PendingDefenseDirection = ECombatDirection::None;
		return;
	}

	CurrentCombatAction = ECombatAction::Defending;
	CurrentCombatDirection = DefenseDirection;
	UE_LOG(LogTemp, Warning, TEXT("Defense: %s | State: Defending/%s"),
		*UEnum::GetValueAsString(DefenseDirection),
		*UEnum::GetValueAsString(DefenseDirection));

	GetWorldTimerManager().SetTimer(CombatStateResetTimerHandle, this, &AEinherjarCharacter::ResetCombatState, AttackDuration, false);
	PendingDefenseDirection = ECombatDirection::None;
}

// ============================================================
// HEALTH SYSTEM
// ============================================================

void AEinherjarCharacter::TakeDamage(float DamageAmount)
{
	if (bIsDead || DamageAmount <= 0.0f) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("%s took %.1f damage | HP: %.1f / %.1f"),
		*GetName(), DamageAmount, CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}
}

void AEinherjarCharacter::Die()
{
	if (bIsDead) return;

	bIsDead = true;
	CurrentHealth = 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("%s died!"), *GetName());

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}
	if (bAutoRespawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s will respawn in %.1fs"), *GetName(), RespawnDelay);
		GetWorldTimerManager().SetTimer(
			RespawnTimerHandle,
			this,
			&AEinherjarCharacter::Respawn,
			RespawnDelay,
			false
		);
	}
}

bool AEinherjarCharacter::IsAlive() const
{
	return !bIsDead;
}

void AEinherjarCharacter::Respawn()
{
	if (!bIsDead) return;

	bIsDead = false;
	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	CurrentCombatAction = ECombatAction::None;
	CurrentCombatDirection = ECombatDirection::None;
	PendingAttackDirection = ECombatDirection::None;
	PendingDefenseDirection = ECombatDirection::None;

	UE_LOG(LogTemp, Warning, TEXT("%s respawned with %.1f HP"), *GetName(), CurrentHealth);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		EnableInput(PC);
	}
}

// ============================================================
// STAMINA SYSTEM
// ============================================================

bool AEinherjarCharacter::ConsumeStamina(float Amount)
{
	if (CurrentStamina < Amount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough stamina! (%.1f / %.1f needed)"), CurrentStamina, Amount);
		return false;
	}

	CurrentStamina = FMath::Clamp(CurrentStamina - Amount, 0.0f, MaxStamina);
	LastStaminaUseTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Warning, TEXT("Stamina: -%.1f | %.1f / %.1f"), Amount, CurrentStamina, MaxStamina);
	return true;
}

float AEinherjarCharacter::GetStaminaPercent() const
{
	return (MaxStamina > 0.0f) ? (CurrentStamina / MaxStamina) : 0.0f;
}

// ============================================================
// DEFENSE LOGIC
// ============================================================

bool AEinherjarCharacter::CanBlockAttack(ECombatDirection IncomingDirection) const
{
	if (CurrentCombatAction != ECombatAction::Defending) return false;

	switch (CurrentCombatDirection)
	{
	case ECombatDirection::Left:
		return IncomingDirection == ECombatDirection::Left;

	case ECombatDirection::Right:
		return IncomingDirection == ECombatDirection::Right;

	case ECombatDirection::Center:
		return IncomingDirection == ECombatDirection::Up
			|| IncomingDirection == ECombatDirection::Down;

	default:
		return false;
	}
}

void AEinherjarCharacter::SetForceDefense(ECombatDirection DefenseDirection)
{
	CurrentCombatAction = ECombatAction::Defending;
	CurrentCombatDirection = DefenseDirection;

	GetWorldTimerManager().ClearTimer(CombatStateResetTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("%s forced defense: %s"),
		*GetName(), *UEnum::GetValueAsString(DefenseDirection));
}

// ============================================================
// ATTACK TRACING
// ============================================================

void AEinherjarCharacter::PerformAttackTrace()
{
	if (bIsDead) return;

	const FVector StartLocation = GetActorLocation() + GetActorForwardVector() * 50.0f;

	const FVector EndLocation = StartLocation + GetActorForwardVector() * AttackRange;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<FHitResult> HitResults;

	const bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		StartLocation,
		EndLocation,
		AttackRadius,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.0f
	);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trace touched %d actor(s)"), HitResults.Num());

		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor) continue;

			if (AEinherjarCharacter* HitCharacter = Cast<AEinherjarCharacter>(HitActor))
			{				
				if (HitCharacter->CanBlockAttack(CurrentCombatDirection))
				{
					UE_LOG(LogTemp, Warning, TEXT("BLOCKED! %s blocked %s attack"),
						*HitCharacter->GetName(), *UEnum::GetValueAsString(CurrentCombatDirection));					
				}
				else
				{
					const float DamageToApply = bWasLastAttackHeavy ? HeavyAttackDamage : AttackDamage;
					HitCharacter->TakeDamage(DamageToApply);
					UE_LOG(LogTemp, Warning, TEXT("Hit (%s): %s for %.1f damage"),
						bWasLastAttackHeavy ? TEXT("HEAVY") : TEXT("LIGHT"),
						*HitCharacter->GetName(),
						DamageToApply);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack missed (no target in range)"));
	}
}

// ============================================================
// ATTACK EXECUTION
// ============================================================

void AEinherjarCharacter::ExecuteAttack(ECombatDirection Direction, bool bHeavy)
{
	if (CurrentCombatAction != ECombatAction::None) return;

	const float Cost = bHeavy ? HeavyAttackStaminaCost : LightAttackStaminaCost;
	if (!ConsumeStamina(Cost))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack failed: not enough stamina (%.1f needed, %.1f available)"),
			Cost, CurrentStamina);
		return;
	}

	CurrentCombatAction = ECombatAction::Attacking;
	CurrentCombatDirection = Direction;
	bWasLastAttackHeavy = bHeavy;

	UE_LOG(LogTemp, Warning, TEXT("Attack: %s | %s | State: Attacking/%s"),
		*UEnum::GetValueAsString(Direction),
		bHeavy ? TEXT("HEAVY") : TEXT("LIGHT"),
		*UEnum::GetValueAsString(Direction));
	
	const float Duration = bHeavy ? AttackDuration * 1.5f : AttackDuration;
	GetWorldTimerManager().SetTimer(CombatStateResetTimerHandle, this, &AEinherjarCharacter::ResetCombatState, Duration, false);

	const float HitDelay = bHeavy ? AttackHitDelay * 1.5f : AttackHitDelay;
	GetWorldTimerManager().SetTimer(AttackHitTimerHandle, this, &AEinherjarCharacter::PerformAttackTrace, HitDelay, false);

	PendingAttackDirection = ECombatDirection::None;
}