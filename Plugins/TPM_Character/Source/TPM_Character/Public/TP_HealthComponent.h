// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_HealthComponent.generated.h"

class ATPCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthRegen, float, RegenAmount);


UCLASS( ClassGroup=(CharacterAttributes), blueprintable, meta=(BlueprintSpawnableComponent))
class TPM_CHARACTER_API UTP_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTP_HealthComponent();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ChangeHealth(float ChangeAmount);

	void SetHelthRegenModifer(float NewModifer);

	UPROPERTY(BlueprintAssignable)
	FOnHealthRegen Health_OnRegen;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Initalize();

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	bool bHealthRegenerates;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float HealthRegenFrequency;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float HealthRegenAmount;

	UPROPERTY(Replicated)
	float HealthRegenModifer;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	FTimerHandle HealthRegenTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	ATPCharacter* MyOwner;

	UFUNCTION()
	void OnDamangeTaken(float DamangeAmount);
	
	UFUNCTION()
	void OnRep_Health();

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnDeath();

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnHealthUpdate();

	void Internal_OnDeath();

	void StartHealthRegen();
	
	void StopHealthRegen();

	void RegenHealth();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ChangeHealth(float ChangeAmount);
	bool Server_ChangeHealth_Validate(float ChangeAmount);
	void Server_ChangeHealth_Implementation(float ChangeAmount);


};
