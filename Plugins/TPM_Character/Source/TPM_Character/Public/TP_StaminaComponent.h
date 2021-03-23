// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_StaminaComponent.generated.h"

class ATPCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaUsed, float, StaminaUsedAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaRegen, float, StaminaRegenAmount);


UCLASS( ClassGroup=(CharacterAttributes), blueprintable, meta=(BlueprintSpawnableComponent) )
class TPM_CHARACTER_API UTP_StaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTP_StaminaComponent();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void ChangeStamina(float StaminaChange);

	void SetStaminaUseModifer(float NewModifer);

	void SetStaminaRegenModifer(float NewModifer);

	UFUNCTION(BlueprintImplementableEvent, Category = "Stamina")
	void StaminaUpdated();

	UPROPERTY(BlueprintAssignable)
	FOnStaminaUsed Stamina_OnUsed;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaRegen Stamina_OnRegen;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stamina")
	float MinStaminaToSprint;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stamina")
	float SprintDecayAmount;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stamina")
	float SprintDecayFrequency;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stamina")
	float StamRegenAmount;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stamina")
	float StamRegenFrequency;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Stamina, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;

	UPROPERTY(Replicated)
	bool bBlockingSprint = false;

	UPROPERTY(Replicated)
	float StaminaUseModifer;

	UPROPERTY(Replicated)
	float StaminaRegenModifer;

	UFUNCTION()
	void OnRep_Stamina();

	UPROPERTY()
	float DefaultSprintModifer;

	FTimerHandle SprintDecayTimer;

	FTimerHandle RegenTimer;

	UPROPERTY()
	ATPCharacter* MyOwner;

	UFUNCTION()
	void Initalize();

	UFUNCTION()
	void OnWantToSprintStart();

	UFUNCTION()
	void OnWantToSprintStop();

	UFUNCTION()
	void DecaySprintStam();

	UFUNCTION()
	void ReGenStam();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ChangeStamina(float StaminaChange);
	bool Server_ChangeStamina_Validate(float StaminaChange);
	void Server_ChangeStamina_Implementation(float StaminaChange);
		
};
