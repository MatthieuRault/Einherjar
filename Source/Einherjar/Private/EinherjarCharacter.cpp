// Fill out your copyright notice in the Description page of Project Settings.

#include "EinherjarCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"

// Sets default values
AEinherjarCharacter::AEinherjarCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
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
		// Bind the 4 directional attacks
		// Started = triggered when the key is pressed
		if (IA_Overhead)
		{
			EnhancedInput->BindAction(IA_Overhead, ETriggerEvent::Started, this, &AEinherjarCharacter::OnOverhead);
		}

		if (IA_Stab)
		{
			EnhancedInput->BindAction(IA_Stab, ETriggerEvent::Started, this, &AEinherjarCharacter::OnStab);
		}

		if (IA_LeftSlash)
		{
			EnhancedInput->BindAction(IA_LeftSlash, ETriggerEvent::Started, this, &AEinherjarCharacter::OnLeftSlash);
		}

		if (IA_RightSlash)
		{
			EnhancedInput->BindAction(IA_RightSlash, ETriggerEvent::Started, this, &AEinherjarCharacter::OnRightSlash);
		}
	}
}

// ============================================================
// COMBAT — HANDLERS
// ============================================================

void AEinherjarCharacter::OnOverhead()
{
	UE_LOG(LogTemp, Warning, TEXT("Combat: Overhead Attack!"));
}

void AEinherjarCharacter::OnStab()
{
	UE_LOG(LogTemp, Warning, TEXT("Combat: Stab Attack!"));
}

void AEinherjarCharacter::OnLeftSlash()
{
	UE_LOG(LogTemp, Warning, TEXT("Combat: Left Slash!"));
}

void AEinherjarCharacter::OnRightSlash()
{
	UE_LOG(LogTemp, Warning, TEXT("Combat: Right Slash!"));
}