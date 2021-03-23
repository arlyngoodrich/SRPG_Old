// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_NutritionComponent.h"
#include "TPCharacter.h"
#include "TPM_Character.h"
#include "TP_StaminaComponent.h"
#include "TP_HealthComponent.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UTP_NutritionComponent::UTP_NutritionComponent()
{

	SetIsReplicated(true);

	MaxFood = 400.f;
	StarveThreshold = 0.2;
	OvereatThreashold = 0.8;
	
	MaxProtein = 100.f;
	ProteinFoodValue = 5.f;
	ProteinBuffPercentThreshold = 0.5;
	StaminaUseCoeffient = 0.1;
	StaminaBuffPercent = 0.8;
	StaminaNerfPercent = 1.2;

	MaxVitamins = 100.f;
	VitaminFoodValue = 2.f;
	VitaminBuffPercentThreshold = 0.5;
	HealthRegenUseCoeffient = 0.1;
	HealthRegenBuffPercent = 1.2;
	HealthRegenNerfPercent = 0.8;

	MaxCarbs = 100.f;
	CarbFoodValue = 10.f;
	CarbsBuffPercentThreshold = 0.5;
	FoodDecreaseBuffPercent = 0.8;
	FoodDecreaseNerfPercent = 1.2;

	MaxWater = 100.f;
	WaterBuffPercentThreshold = 0.5;
	StaminaRegenBuffPercent = 1.2;
	StaminaRegenNerfPercent = 0.8;
	WaterRegenUseCoeffient = 0.2;


	MetabolicFrequency = 1.f;
	DefaultFoodChangeAmount = 1.f;
	DefaultProteinChangeAmount = 1.f;
	DefaultVitaminChangeAmount = 1.f;
	DefaultCarbsChangeAmount = 1.f;
	DefaultWaterChangeAmount = 1.f;

	// ...
}




// Called when the game starts
void UTP_NutritionComponent::BeginPlay()
{
	Super::BeginPlay();

	Initalize();
	
}

void UTP_NutritionComponent::Initalize()
{

	if (Cast<ATPCharacter>(GetOwner()))
	{
		MyOwner = Cast<ATPCharacter>(GetOwner());

		HealthComponent = MyOwner->FindComponentByClass<UTP_HealthComponent>();
		StaminaComponent = MyOwner->FindComponentByClass<UTP_StaminaComponent>();

		if (!HealthComponent || !StaminaComponent)
		{
			UE_LOG(LogTPCharacter, Error, TEXT("Nutrition could not set health or stamina components"))
		}
		else
		{
			HealthComponent->Health_OnRegen.AddDynamic(this, &UTP_NutritionComponent::OnHealthRegen);
			StaminaComponent->Stamina_OnUsed.AddDynamic(this, &UTP_NutritionComponent::OnStaminaUsed);
			StaminaComponent->Stamina_OnRegen.AddDynamic(this, &UTP_NutritionComponent::OnStaminaRegen);
			UE_LOG(LogTPCharacter, Log, TEXT("Nutrition component set"))

		}

		CurrentFood = StartingFood;
		CurrentProtein = MaxProtein;
		CurrentVitamins = MaxVitamins;
		CurrentCarbs = MaxCarbs;
		CurrentWater = MaxWater;
		CurrentFoodChangeAmount = DefaultFoodChangeAmount;


		if(GetOwnerRole() == ROLE_Authority)
		{
			GetWorld()->GetTimerManager().SetTimer(BaseMetabolismTimer, this, &UTP_NutritionComponent::Metabolize, MetabolicFrequency, true, 0.f);
		}
	}


}


void UTP_NutritionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTP_NutritionComponent, MaxFood);
	DOREPLIFETIME(UTP_NutritionComponent, CurrentFood);
	DOREPLIFETIME(UTP_NutritionComponent, StarveThreshold);
	DOREPLIFETIME(UTP_NutritionComponent, OvereatThreashold);
	DOREPLIFETIME(UTP_NutritionComponent, MaxProtein);
	DOREPLIFETIME(UTP_NutritionComponent, CurrentProtein);
	DOREPLIFETIME(UTP_NutritionComponent, ProteinBuffPercentThreshold);
	DOREPLIFETIME(UTP_NutritionComponent, MaxVitamins);
	DOREPLIFETIME(UTP_NutritionComponent, CurrentVitamins);
	DOREPLIFETIME(UTP_NutritionComponent, VitaminBuffPercentThreshold);
	DOREPLIFETIME(UTP_NutritionComponent, MaxCarbs);
	DOREPLIFETIME(UTP_NutritionComponent, CurrentCarbs);
	DOREPLIFETIME(UTP_NutritionComponent, CarbsBuffPercentThreshold);
	DOREPLIFETIME(UTP_NutritionComponent, MetabolicFrequency);
	DOREPLIFETIME(UTP_NutritionComponent, StaminaUseCoeffient);
	DOREPLIFETIME(UTP_NutritionComponent, HealthRegenUseCoeffient);
	DOREPLIFETIME(UTP_NutritionComponent, ProteinFoodValue);
	DOREPLIFETIME(UTP_NutritionComponent, VitaminFoodValue);
	DOREPLIFETIME(UTP_NutritionComponent, CarbFoodValue);
	DOREPLIFETIME(UTP_NutritionComponent, StaminaBuffPercent);
	DOREPLIFETIME(UTP_NutritionComponent, StaminaNerfPercent);
	DOREPLIFETIME(UTP_NutritionComponent, HealthRegenBuffPercent);
	DOREPLIFETIME(UTP_NutritionComponent, HealthRegenNerfPercent);
	DOREPLIFETIME(UTP_NutritionComponent, FoodDecreaseBuffPercent);
	DOREPLIFETIME(UTP_NutritionComponent, FoodDecreaseNerfPercent);
	DOREPLIFETIME(UTP_NutritionComponent, DefaultFoodChangeAmount);
	DOREPLIFETIME(UTP_NutritionComponent, DefaultProteinChangeAmount);
	DOREPLIFETIME(UTP_NutritionComponent, DefaultVitaminChangeAmount);
	DOREPLIFETIME(UTP_NutritionComponent, DefaultCarbsChangeAmount);
	DOREPLIFETIME(UTP_NutritionComponent, CurrentFoodChangeAmount);
	DOREPLIFETIME(UTP_NutritionComponent, MaxWater);
	DOREPLIFETIME(UTP_NutritionComponent, CurrentWater);
	DOREPLIFETIME(UTP_NutritionComponent, WaterBuffPercentThreshold);
	DOREPLIFETIME(UTP_NutritionComponent, StaminaRegenBuffPercent);
	DOREPLIFETIME(UTP_NutritionComponent, StaminaRegenNerfPercent);
	DOREPLIFETIME(UTP_NutritionComponent, DefaultWaterChangeAmount);

}

void UTP_NutritionComponent::AddNutrition(float Protein, float Vitamins, float Carbs, float Water)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		float FoodToAdd;

		FoodToAdd = (Protein * ProteinFoodValue) + (Vitamins * VitaminFoodValue) + (Carbs * CarbFoodValue);
	
		CurrentFood = FMath::Clamp(FoodToAdd + CurrentFood, 0.f, MaxFood);
		CurrentProtein = FMath::Clamp(Protein + CurrentProtein, 0.f, MaxProtein);
		CurrentVitamins = FMath::Clamp(Vitamins + CurrentVitamins, 0.f, MaxVitamins);
		CurrentCarbs = FMath::Clamp(Carbs + CurrentCarbs, 0.f, MaxCarbs);
		CurrentWater = FMath::Clamp(Water + CurrentWater, 0.f, MaxWater);
		
		ApplyStates();

	}
	else
	{
		Server_AddNutrition(Protein, Vitamins, Carbs, Water);
	}

}

void UTP_NutritionComponent::GetNutritionStats(float& OutCurrentFood, float& OutCurrentProtein, float& OutCurrentVitamins, float& OutCurrentCarbs, float& OutCurrentWater)
{
	OutCurrentFood = CurrentFood;
	OutCurrentProtein = CurrentProtein;
	OutCurrentVitamins = CurrentVitamins;
	OutCurrentCarbs = CurrentCarbs;
	OutCurrentWater = CurrentWater;
}

void UTP_NutritionComponent::GetMaxNutritionStats(float& OutMaxFood, float& OutMaxProtein, float& OutMaxVitamins, float& OutMaxCarbs, float& OutMaxWater)
{
	OutMaxFood = MaxFood;
	OutMaxProtein = MaxProtein;
	OutMaxVitamins = MaxVitamins;
	OutMaxCarbs = MaxCarbs;
	OutMaxWater = MaxWater;
}

void UTP_NutritionComponent::GetNutritionPercents(float& OutFoodPercent, float& OutProteinPercent, float& OutVitaminPerecnt, float& OutCarbPercent, float& OutWaterPercent)
{
	OutFoodPercent = CurrentFood / MaxFood;
	OutProteinPercent = CurrentProtein / MaxProtein;
	OutVitaminPerecnt = CurrentVitamins / MaxVitamins;
	OutCarbPercent = CurrentCarbs / MaxCarbs;
	OutWaterPercent = CurrentWater / MaxWater;

}


void UTP_NutritionComponent::Metabolize()
{
	CurrentFood = FMath::Clamp(CurrentFood - CurrentFoodChangeAmount, 0.f, MaxFood);
	CurrentProtein = FMath::Clamp(CurrentProtein - DefaultProteinChangeAmount, 0.f, MaxProtein);
	CurrentVitamins = FMath::Clamp(CurrentVitamins - DefaultVitaminChangeAmount, 0.f, MaxVitamins);
	CurrentCarbs = FMath::Clamp(CurrentCarbs - DefaultCarbsChangeAmount, 0.f, MaxCarbs);
	CurrentWater = FMath::Clamp(CurrentWater - DefaultWaterChangeAmount, 0.f, MaxWater);

	ApplyStates();

}

void UTP_NutritionComponent::OnHealthRegen(float RegenAmount)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOG(LogTPCharacter, Log, TEXT("Vitamins lost on Health Regen: %s"),*FString::SanitizeFloat(RegenAmount * HealthRegenUseCoeffient))

		CurrentVitamins = FMath::Clamp(CurrentVitamins - (RegenAmount * HealthRegenUseCoeffient), 0.f, MaxVitamins);
		ApplyStates();
	}
}

void UTP_NutritionComponent::OnStaminaUsed(float StaminaAmount)
{
	if (GetOwnerRole() == ROLE_Authority)
	{

		UE_LOG(LogTPCharacter, Log, TEXT("Protein lost on Stamina Use: %s"), *FString::SanitizeFloat(StaminaAmount * StaminaUseCoeffient))

		//Should be negative stamina if being used
		CurrentProtein = FMath::Clamp(CurrentProtein - (StaminaAmount * StaminaUseCoeffient), 0.f, MaxProtein);
		ApplyStates();
	}
}

void UTP_NutritionComponent::OnStaminaRegen(float StaminaAmount)
{
	if (GetOwnerRole() == ROLE_Authority)
	{

		UE_LOG(LogTPCharacter, Log, TEXT("Water lost on Stamina Regen: %s"), *FString::SanitizeFloat(StaminaAmount * WaterRegenUseCoeffient))

			//Should be negative stamina if being used
		CurrentWater = FMath::Clamp(CurrentWater - (StaminaAmount * WaterRegenUseCoeffient), 0.f, MaxWater);
		ApplyStates();
	}



}

void UTP_NutritionComponent::OnRep_Food()
{
	Internal_NutritionUpdated();
}

void UTP_NutritionComponent::OnRep_Protein()
{

	Internal_NutritionUpdated();
}

void UTP_NutritionComponent::OnRep_Vitamins()
{
	Internal_NutritionUpdated();
}

void UTP_NutritionComponent::OnRep_Carbs()
{

	Internal_NutritionUpdated();
}

void UTP_NutritionComponent::OnRep_Water()
{
	Internal_NutritionUpdated();
}

void UTP_NutritionComponent::Internal_NutritionUpdated()
{
	Nutrition_OnUpdate.Broadcast();
	NutritionUpdated();
}



void UTP_NutritionComponent::ApplyStates()
{
	if (CurrentFood < StarveThreshold*MaxFood)
	{
		UE_LOG(LogTPCharacter,Log,TEXT("Character is starving"))
	}
	else if (CurrentFood > OvereatThreashold*MaxFood)
	{
		UE_LOG(LogTPCharacter, Log, TEXT("Character has overeaten"))
	}


	if (StaminaComponent)
	{
		if (CurrentProtein > MaxProtein * ProteinBuffPercentThreshold)
		{
			//Apply Protein Buff
			StaminaComponent->SetStaminaUseModifer(StaminaBuffPercent);
		}
		else
		{
			//Apply Protein Nerf
			StaminaComponent->SetStaminaUseModifer(StaminaNerfPercent);
		}
	}

	if (HealthComponent)
	{
		if (CurrentVitamins > MaxVitamins * VitaminBuffPercentThreshold)
		{
			//Apply Vitamin Buff
			HealthComponent->SetHelthRegenModifer(HealthRegenBuffPercent);

		}
		else
		{
			//Apply Vitamin Nerf
			HealthComponent->SetHelthRegenModifer(HealthRegenNerfPercent);
		}
	}

	if (CurrentCarbs > MaxCarbs * CarbsBuffPercentThreshold)
	{
		//Apply Carb Buff
		CurrentFoodChangeAmount = DefaultFoodChangeAmount * FoodDecreaseBuffPercent;
	}
	else
	{
		//Apply Carb Nerf
		CurrentFoodChangeAmount = DefaultFoodChangeAmount * FoodDecreaseNerfPercent;
	}

	if (StaminaComponent)
	{
		if (CurrentProtein > MaxProtein * ProteinBuffPercentThreshold)
		{
			//Apply Water Buff
			StaminaComponent->SetStaminaRegenModifer(StaminaRegenBuffPercent);
		}
		else
		{
			//Apply Water Nerf
			StaminaComponent->SetStaminaRegenModifer(StaminaRegenNerfPercent);
		}
	}


	Internal_NutritionUpdated();

	UE_LOG(LogTPCharacter, Verbose, TEXT("Food: %s"), *FString::SanitizeFloat(CurrentFood))
	UE_LOG(LogTPCharacter, Verbose, TEXT("Protein: %s"), *FString::SanitizeFloat(CurrentProtein))
	UE_LOG(LogTPCharacter, Verbose, TEXT("Vitamins: %s"), *FString::SanitizeFloat(CurrentVitamins))
	UE_LOG(LogTPCharacter, Verbose, TEXT("Carbs: %s"), *FString::SanitizeFloat(CurrentCarbs))


}


bool UTP_NutritionComponent::Server_AddNutrition_Validate(float Protein, float Vitamins, float Carbs, float Water)
{
	return true;
}

void UTP_NutritionComponent::Server_AddNutrition_Implementation(float Protein, float Vitamins, float Carbs, float Water)
{
	AddNutrition(Protein, Vitamins, Carbs, Water);
}