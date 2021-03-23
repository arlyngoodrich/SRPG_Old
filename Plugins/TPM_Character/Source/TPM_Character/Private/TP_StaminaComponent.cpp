// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_StaminaComponent.h"
#include "TPCharacter.h"
#include "TPM_Character.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UTP_StaminaComponent::UTP_StaminaComponent()
{

	SetIsReplicated(true);
	MinStaminaToSprint = 20.f;
	SprintDecayAmount = -1.f;
	SprintDecayFrequency = 1.f;
	StamRegenAmount = 1.f;
	StamRegenFrequency = 1.f;
	MaxStamina = 100.f;
	StaminaUseModifer = 1.f;
	StaminaRegenModifer = 1.f;
}



// Called when the game starts
void UTP_StaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	Initalize();
	
}




void UTP_StaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTP_StaminaComponent, CurrentStamina);
	DOREPLIFETIME(UTP_StaminaComponent, MaxStamina);
	DOREPLIFETIME(UTP_StaminaComponent, bBlockingSprint);
	DOREPLIFETIME(UTP_StaminaComponent, MinStaminaToSprint);
	DOREPLIFETIME(UTP_StaminaComponent, SprintDecayAmount);
	DOREPLIFETIME(UTP_StaminaComponent, SprintDecayFrequency);
	DOREPLIFETIME(UTP_StaminaComponent, StamRegenAmount);
	DOREPLIFETIME(UTP_StaminaComponent, StamRegenFrequency);
	DOREPLIFETIME(UTP_StaminaComponent, StaminaUseModifer);
	DOREPLIFETIME(UTP_StaminaComponent, StaminaRegenModifer);
	
}

void UTP_StaminaComponent::OnRep_Stamina()
{
	UE_LOG(LogTPCharacter, Verbose, TEXT("On_Rep Stamina Change called"))
	StaminaUpdated();
}

void UTP_StaminaComponent::Initalize()
{

	CurrentStamina = MaxStamina;

	if (Cast<ATPCharacter>(GetOwner()))
	{
		MyOwner = Cast<ATPCharacter>(GetOwner());
		MyOwner->WantsToSprint_OnSprintStart.AddDynamic(this, &UTP_StaminaComponent::OnWantToSprintStart);
		MyOwner->WantsToNotSprint_OnSptrintStop.AddDynamic(this, &UTP_StaminaComponent::OnWantToSprintStop);
		DefaultSprintModifer = MyOwner->GetSprintSpeedModifier();
		UE_LOG(LogTPCharacter, Log, TEXT("Stamina Component is set"))

			if (GetOwnerRole() == ROLE_Authority)
			{
				GetWorld()->GetTimerManager().SetTimer(RegenTimer, this, &UTP_StaminaComponent::ReGenStam, StamRegenFrequency, true, 0.f);
				GetWorld()->GetTimerManager().PauseTimer(RegenTimer);
			}
	

	}
	else
	{
		UE_LOG(LogTPCharacter, Error, TEXT("Stamina Component could not find owner"))
	}

}

void UTP_StaminaComponent::OnWantToSprintStart()
{
	UE_LOG(LogTPCharacter, Log, TEXT("Stamina Comp: Sprint Started"))

		if (RegenTimer.IsValid())
		{
			GetWorld()->GetTimerManager().PauseTimer(RegenTimer);
		}

		if (SprintDecayTimer.IsValid())
		{
			GetWorld()->GetTimerManager().UnPauseTimer(SprintDecayTimer);
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(SprintDecayTimer, this, &UTP_StaminaComponent::DecaySprintStam, SprintDecayFrequency, true, 0.f);
		}
	
}



void UTP_StaminaComponent::OnWantToSprintStop()
{
	UE_LOG(LogTPCharacter, Log, TEXT("Stamina Comp: Sprint Stopped"))

	if(SprintDecayTimer.IsValid())
	{
		GetWorld()->GetTimerManager().PauseTimer(SprintDecayTimer);
	}
	
	if (RegenTimer.IsValid())
	{
		GetWorld()->GetTimerManager().UnPauseTimer(RegenTimer);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(RegenTimer, this, &UTP_StaminaComponent::ReGenStam, StamRegenFrequency, true, 0.f);
	}


}

void UTP_StaminaComponent::DecaySprintStam()
{

		if (CurrentStamina <= MinStaminaToSprint)
		{
			MyOwner->SetSprintSpeedModifer(1.f);
			MyOwner->SetWantsToSprint();
			bBlockingSprint = true;
		}
		else
		{
			ChangeStamina(SprintDecayAmount);
		}

		UE_LOG(LogTPCharacter, Verbose, TEXT("Decay Sprint Stam -- Current Stamina: %s"), *FString::SanitizeFloat(CurrentStamina))

}

void UTP_StaminaComponent::ReGenStam()
{
	ChangeStamina(StamRegenAmount* StaminaRegenModifer);
	Stamina_OnRegen.Broadcast(StamRegenAmount * StaminaRegenModifer);


		if (bBlockingSprint == true)
		{
			if (CurrentStamina >= MinStaminaToSprint)
			{
				MyOwner->SetSprintSpeedModifer(DefaultSprintModifer);
				bBlockingSprint = false;
			}
		}

		UE_LOG(LogTPCharacter, Verbose, TEXT(" Regen Stam -- Current Stamina: %s"), *FString::SanitizeFloat(CurrentStamina))
}



void UTP_StaminaComponent::ChangeStamina(float StaminaChange)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_ChangeStamina(StaminaChange);
	}
	else
	{
		float OldStamina;
		OldStamina = CurrentStamina;

		CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaChange* StaminaUseModifer), 0.f, MaxStamina);

		//Broadcast Stamina Usage
		if (StaminaChange < 0)
		{
			Stamina_OnUsed.Broadcast(OldStamina - CurrentStamina);
		}
		
		//Stop Regen if at Max
		if (CurrentStamina == MaxStamina)
		{
			GetWorld()->GetTimerManager().PauseTimer(RegenTimer);
		}

		UE_LOG(LogTPCharacter, Log, TEXT("Net Stam used: %s"), *FString::SanitizeFloat(StaminaChange * StaminaUseModifer))
		StaminaUpdated();
	}

}

void UTP_StaminaComponent::SetStaminaUseModifer(float NewModifer)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		StaminaUseModifer = NewModifer;
	}

}

void UTP_StaminaComponent::SetStaminaRegenModifer(float NewModifer)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		StaminaRegenModifer = NewModifer;
	}

}


bool UTP_StaminaComponent::Server_ChangeStamina_Validate(float StaminaChange)
{
	return true;
}

void UTP_StaminaComponent::Server_ChangeStamina_Implementation(float StaminaChange)
{
	ChangeStamina(StaminaChange);
}
