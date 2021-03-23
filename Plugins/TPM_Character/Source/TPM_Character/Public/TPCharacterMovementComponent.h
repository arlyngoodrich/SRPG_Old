// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPCharacterMovementComponent.generated.h"

/**
 * 
 */

class ATPCharacter;

UCLASS()
class TPM_CHARACTER_API UTPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;

protected:

	ATPCharacter* OwningCharacter;

	float DefaultMaxSpeed;
	
};
