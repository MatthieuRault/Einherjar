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

	// Health initialization
	CurrentHealth = MaxHealth;
	bIsDead = false;
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
	if (CurrentCombatAction != ECombatAction::None) return;
	CurrentCombatAction = ECombatAction::Attacking;
	CurrentCombatDirection = ECombatDirection::Up;
	UE_LOG(LogTemp, Warning, TEXT("Attack: Overhead | State: Attacking/Up"));
	GetWorldTimerManager().SetTimer(CombatStateResetTimerHandle, this, &AEinherjarCharacter::ResetCombatState, AttackDuration, false);
	GetWorldTimerManager().SetTimer(AttackHitTimerHandle, this, &AEinherjarCharacter::PerformAttackTrace, AttackHitDelay, false);
}

void AEinherjarCharacter::OnStab()
{
	if (CurrentCombatAction != ECombatAction::None) return;
	CurrentCombatAction = ECombatAction::Attacking;
	CurrentCombatDirection = ECombatDirection::Down;
	UE_LOG(LogTemp, Warning, TEXT("Attack: Stab | State: Attacking/Down"));
	GetWorldTimerManager().SetTimer(CombatStateResetTimerHandle, this, &AEinherjarCharacter::ResetCombatState, AttackDuration, false);
	GetWorldTimerManager().SetTimer(AttackHitTimerHandle, this, &AEinherjarCharacter::PerformAttackTrace, AttackHitDelay, false);
}

void AEinherjarCharacter::OnLeftSlash()
{
	if (CurrentCombatAction != ECombatAction::None) return;
	CurrentCombatAction = ECombatAction::Attacking;
	CurrentCombatDirection = ECombatDirection::Left;
	UE_LOG(LogTemp, Warning, TEXT("Attack: Left Slash | State: Attacking/Left"));
	GetWorldTimerManager().SetTimer(CombatStateResetTimerHandle, this, &AEinherjarCharacter::ResetCombatState, AttackDuration, false);
	GetWorldTimerManager().SetTimer(AttackHitTimerHandle, this, &AEinherjarCharacter::PerformAttackTrace, AttackHitDelay, false);
}

void AEinherjarCharacter::OnRightSlash()
{
	if (CurrentCombatAction != ECombatAction::None) return;
	CurrentCombatAction = ECombatAction::Attacking;
	CurrentCombatDirection = ECombatDirection::Right;
	UE_LOG(LogTemp, Warning, TEXT("Attack: Right Slash | State: Attacking/Right"));
	GetWorldTimerManager().SetTimer(CombatStateResetTimerHandle, this, &AEinherjarCharacter::ResetCombatState, AttackDuration, false);
	GetWorldTimerManager().SetTimer(AttackHitTimerHandle, this, &AEinherjarCharacter::PerformAttackTrace, AttackHitDelay, false);
}

void AEinherjarCharacter::OnKick()
{
	UE_LOG(LogTemp, Warning, TEXT("Kick! (l'etat ne change pas)"));
}

void AEinherjarCharacter::OnAttackCancel()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack Cancel | State: None"));
	GetWorldTimerManager().ClearTimer(CombatStateResetTimerHandle);
	GetWorldTimerManager().ClearTimer(AttackHitTimerHandle);
	ResetCombatState();
}

// ============================================================
// DEFENSES
// ============================================================

void AEinherjarCharacter::OnDefenseLeft()
{
	if (CurrentCombatAction != ECombatAction::None) return;
	CurrentCombatAction = ECombatAction::Defending;
	CurrentCombatDirection = ECombatDirection::Left;
	UE_LOG(LogTemp, Warning, TEXT("Defense: Left | State: Defending/Left"));
	GetWorldTimerManager().SetTimer(CombatStateResetTimerHandle, this, &AEinherjarCharacter::ResetCombatState, AttackDuration, false);
}

void AEinherjarCharacter::OnDefenseCenter()
{
	if (CurrentCombatAction != ECombatAction::None) return;
	CurrentCombatAction = ECombatAction::Defending;
	CurrentCombatDirection = ECombatDirection::Center;
	UE_LOG(LogTemp, Warning, TEXT("Defense: Center | State: Defending/Center"));
	GetWorldTimerManager().SetTimer(CombatStateResetTimerHandle, this, &AEinherjarCharacter::ResetCombatState, AttackDuration, false);
}

void AEinherjarCharacter::OnDefenseRight()
{
	if (CurrentCombatAction != ECombatAction::None) return;
	CurrentCombatAction = ECombatAction::Defending;
	CurrentCombatDirection = ECombatDirection::Right;
	UE_LOG(LogTemp, Warning, TEXT("Defense: Right | State: Defending/Right"));
	GetWorldTimerManager().SetTimer(CombatStateResetTimerHandle, this, &AEinherjarCharacter::ResetCombatState, AttackDuration, false);
}

// ============================================================
// HELPERS
// ============================================================

void AEinherjarCharacter::ResetCombatState()
{
	CurrentCombatAction = ECombatAction::None;
	CurrentCombatDirection = ECombatDirection::None;
	UE_LOG(LogTemp, Warning, TEXT("Combat state reset to None"));
}

// ============================================================
// MOUSE-BASED COMBAT
// ============================================================

void AEinherjarCharacter::OnMouseAttackStarted()
{
	bIsTrackingAttack = true;
	AccumulatedMouseDelta = FVector2D::ZeroVector;
}

void AEinherjarCharacter::OnMouseAttackReleased()
{
	if (!bIsTrackingAttack) return;
	bIsTrackingAttack = false;

	const float AbsX = FMath::Abs(AccumulatedMouseDelta.X);
	const float AbsY = FMath::Abs(AccumulatedMouseDelta.Y);
	const float MinThreshold = 1.0f;

	if (AbsX < MinThreshold && AbsY < MinThreshold) return;

	if (AbsY > AbsX)
	{
		if (AccumulatedMouseDelta.Y < 0) OnStab();
		else                              OnOverhead();
	}
	else
	{
		if (AccumulatedMouseDelta.X < 0) OnLeftSlash();
		else                              OnRightSlash();
	}
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

	const float AbsX = FMath::Abs(AccumulatedMouseDelta.X);
	const float CenterThreshold = 2.0f;

	if (AbsX < CenterThreshold)         OnDefenseCenter();
	else if (AccumulatedMouseDelta.X < 0) OnDefenseLeft();
	else                                  OnDefenseRight();
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
}

bool AEinherjarCharacter::IsAlive() const
{
	return !bIsDead;
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
					HitCharacter->TakeDamage(AttackDamage);
					UE_LOG(LogTemp, Warning, TEXT("Hit: %s for %.1f damage"), *HitCharacter->GetName(), AttackDamage);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack missed (no target in range)"));
	}
}