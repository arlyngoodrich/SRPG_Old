// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCharacterMovementComponent.h"
#include "TPCharacter.h"


void UTPCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ATPCharacter>(PawnOwner);

	DefaultMaxSpeed = OwningCharacter->GetDefaultWalkSpeed();

}

float UTPCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (OwningCharacter)
	{
		if (OwningCharacter->GetWantsToSprint() && OwningCharacter->bIsCrouched == false)
		{
			MaxSpeed = DefaultMaxSpeed * OwningCharacter->GetSprintSpeedModifier();
		}
		else
		{
			MaxSpeed = DefaultMaxSpeed;
		}
	}

	return MaxSpeed;
}
