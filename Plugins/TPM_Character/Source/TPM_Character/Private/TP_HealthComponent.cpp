// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_HealthComponent.h"
#include "TPCharacter.h"
#include "TPM_Character.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UTP_HealthComponent::UTP_HealthComponent()
{
	SetIsReplicated(true);

	MaxHealth = 100.f;
	bHealthRegenerates = true;
	HealthRegenAmount = 1.f;
	HealthRegenFrequency = 2.f;
	HealthRegenModifer = 1.f;
}



// Called when the game starts
void UTP_HealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Initalize();
	
}

void UTP_HealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTP_HealthComponent, MaxHealth);
	DOREPLIFETIME(UTP_HealthComponent, CurrentHealth);
	DOREPLIFETIME(UTP_HealthComponent, bHealthRegenerates);
	DOREPLIFETIME(UTP_HealthComponent, HealthRegenFrequency);
	DOREPLIFETIME(UTP_HealthComponent, HealthRegenAmount);
	DOREPLIFETIME(UTP_HealthComponent, HealthRegenModifer);
	
	
}


void UTP_HealthComponent::Initalize()
{
	CurrentHealth = MaxHealth;

	if (Cast<ATPCharacter>(GetOwner()))
	{
		MyOwner = Cast<ATPCharacter>(GetOwner());
		MyOwner->Damage_DamageTaken.AddDynamic(this, &UTP_HealthComponent::OnDamangeTaken);
		UE_LOG(LogTPCharacter, Log, TEXT("Health component set"))
	}

}

void UTP_HealthComponent::ChangeHealth(float ChangeAmount)
{
	if (GetOwnerRole() == ROLE_Authority)
	{

		CurrentHealth = FMath::Clamp(CurrentHealth + ChangeAmount, 0.f, MaxHealth);
		OnHealthUpdate();

		UE_LOG(LogTPCharacter, Log, TEXT("Current Health: %s"), *FString::SanitizeFloat(CurrentHealth))

		if(CurrentHealth<=0)
		{
			Internal_OnDeath();
		}

		if (CurrentHealth < MaxHealth)
		{
			StartHealthRegen();
		}
		else
		{
			StopHealthRegen();
		}
	}
	else
	{
		Server_ChangeHealth(ChangeAmount);
	}
}

void UTP_HealthComponent::SetHelthRegenModifer(float NewModifer)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		HealthRegenModifer = NewModifer;
	}
}


void UTP_HealthComponent::OnDamangeTaken(float DamangeAmount)
{
	ChangeHealth(-1 * DamangeAmount);
}

void UTP_HealthComponent::OnRep_Health()
{
	OnHealthUpdate();
}



void UTP_HealthComponent::Internal_OnDeath()
{
	UE_LOG(LogTPCharacter, Log, TEXT("%s character has died"), *MyOwner->GetName())
	OnDeath();
}

void UTP_HealthComponent::StartHealthRegen()
{
	if (HealthRegenTimer.IsValid())
	{
		GetWorld()->GetTimerManager().UnPauseTimer(HealthRegenTimer);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(HealthRegenTimer, this, &UTP_HealthComponent::RegenHealth, HealthRegenFrequency, true, 0);
	}
}

void UTP_HealthComponent::StopHealthRegen()
{
	if (HealthRegenTimer.IsValid())
	{
		GetWorld()->GetTimerManager().PauseTimer(HealthRegenTimer);
	}

}

void UTP_HealthComponent::RegenHealth()
{
	ChangeHealth(HealthRegenAmount * HealthRegenModifer);
	Health_OnRegen.Broadcast(HealthRegenAmount * HealthRegenModifer);

	UE_LOG(LogTPCharacter, Log, TEXT("Net Health Regen Amount: %s"),*FString::SanitizeFloat(HealthRegenAmount*HealthRegenModifer))
}

bool UTP_HealthComponent::Server_ChangeHealth_Validate(float ChangeAmount)
{
	return true;
}

void UTP_HealthComponent::Server_ChangeHealth_Implementation(float ChangeAmount)
{
	ChangeHealth(ChangeAmount);
}
