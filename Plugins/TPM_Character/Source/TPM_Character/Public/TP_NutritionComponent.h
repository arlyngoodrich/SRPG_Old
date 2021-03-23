// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_NutritionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNutritionUpdated);

class ATPCharacter;
class UTP_StaminaComponent;
class UTP_HealthComponent;

UCLASS(ClassGroup = (CharacterAttributes), blueprintable, meta = (BlueprintSpawnableComponent))
class TPM_CHARACTER_API UTP_NutritionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTP_NutritionComponent();

	UFUNCTION(BlueprintCallable, Category = "Nutrition")
	void AddNutrition(float Protein, float Vitamins, float Carbs, float Water);

	UPROPERTY(BlueprintAssignable)
	FOnNutritionUpdated Nutrition_OnUpdate;

	UFUNCTION(BlueprintPure, Category = "Nutrition")
	void GetNutritionStats(float& OutCurrentFood, float& OutCurrentProtein, float& OutCurrentVitamins, float& OutCurrentCarbs, float& OutCurrentWater);
	
	UFUNCTION(BlueprintPure, Category = "Nutrition")
	void GetMaxNutritionStats(float& OutMaxFood, float& OutMaxProtein, float& OutMaxVitamins, float& OutMaxCarbs, float& OutMaxWater);

	UFUNCTION(BlueprintPure, Category = "Nutrition")
	void GetNutritionPercents(float& OutFoodPercent, float& OutProteinPercent, float& OutVitaminPerecnt, float& OutCarbPercent, float& OutWaterPercent);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	// ===== Food Properties ===== 


	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float MaxFood;

	UPROPERTY(ReplicatedUsing = OnRep_Food, Transient, BlueprintReadOnly, Category = "Nutrition")
	float CurrentFood;

	//Percent of Max Food
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float StartingFood;
	
	//Percent of Max Food
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "0.5", UIMin = "0.0", UIMax = "0.5"))
	float StarveThreshold;

	//Percent of Max Food
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.5", ClampMax = "1.0", UIMin = "0.5", UIMax = "1.0"))
	float OvereatThreashold;


	// ==== Protein Properties ====

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float MaxProtein;

	UPROPERTY(ReplicatedUsing = OnRep_Protein, Transient, BlueprintReadOnly, Category = "Nutrition")
	float CurrentProtein;

	//How much fullness for each unit of protein
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float ProteinFoodValue;

	//Protein decrease by each stamin unit burned
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float StaminaUseCoeffient;

	//Perecnt of Max Protein
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float ProteinBuffPercentThreshold;

	// Percent that will be multiplied to stamina usage
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float StaminaBuffPercent;

	// Percent that will be multiplied to stamina usage
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float StaminaNerfPercent;

	// ==== Vitamin Properties ====
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float MaxVitamins;

	UPROPERTY(ReplicatedUsing = OnRep_Vitamins, Transient, BlueprintReadOnly, Category = "Nutrition")
	float CurrentVitamins;

	//How much fullness for each unit of vitamin
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float VitaminFoodValue;

	//Vitamin decrease by each health unit regenerated
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float HealthRegenUseCoeffient;

	//Perecnt of Max Vitamins
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float VitaminBuffPercentThreshold;

	// Percent that will be multiplied to health regen
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float HealthRegenBuffPercent;

	// Percent that will be multiplied to health regen
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float HealthRegenNerfPercent;



	// ==== Carbs Properties ====

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float MaxCarbs;

	UPROPERTY(ReplicatedUsing = OnRep_Carbs, Transient, BlueprintReadOnly, Category = "Nutrition")
	float CurrentCarbs;

	//How much fullness for each unit of carbs
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float CarbFoodValue;

	//Perecnt of Max Protein
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float CarbsBuffPercentThreshold;

	// Percent that will be multiplied to health regen
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float FoodDecreaseBuffPercent;

	// Percent that will be multiplied to health regen
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float FoodDecreaseNerfPercent;
		

	// ==== Water Properties ====

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float MaxWater;

	UPROPERTY(ReplicatedUsing = OnRep_Water, Transient, BlueprintReadOnly, Category = "Nutrition")
	float CurrentWater;

	//Perecnt of Water Protein
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float WaterBuffPercentThreshold;

	// Percent that will be multiplied to health regen
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float StaminaRegenBuffPercent;

	// Percent that will be multiplied to health regen
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float StaminaRegenNerfPercent;

	//Water decrease by each stamina unit regenerated
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float WaterRegenUseCoeffient;

	
	

	// ==== Metabolic Properties ==== 

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float MetabolicFrequency;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float DefaultFoodChangeAmount;

	UPROPERTY(Replicated, Transient, BlueprintReadOnly, Category = "Nutrition")
	float CurrentFoodChangeAmount;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float DefaultProteinChangeAmount;
	
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float DefaultVitaminChangeAmount;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float DefaultCarbsChangeAmount;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Nutrition")
	float DefaultWaterChangeAmount;

	FTimerHandle BaseMetabolismTimer;


	void Initalize();

	UPROPERTY(BlueprintReadOnly, Category = "Nutrition")
	ATPCharacter* MyOwner;

	UPROPERTY(BlueprintReadOnly, Category = "Nutrition")
	UTP_StaminaComponent* StaminaComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Nutrition")
	UTP_HealthComponent* HealthComponent;


	void ApplyStates();

	void Metabolize();

	UFUNCTION()
	void OnHealthRegen(float RegenAmount);

	UFUNCTION()
	void OnStaminaUsed(float StaminaAmount);

	UFUNCTION()
	void OnStaminaRegen(float StaminaAmount);

	UFUNCTION()
	void OnRep_Food();

	UFUNCTION()
	void OnRep_Protein();

	UFUNCTION()
	void OnRep_Vitamins();

	UFUNCTION()
	void OnRep_Carbs();

	UFUNCTION()
	void OnRep_Water();

	void Internal_NutritionUpdated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Nutrition")
	void NutritionUpdated();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddNutrition(float Protein, float Vitamins, float Carbs, float Water);
	bool Server_AddNutrition_Validate(float Protein, float Vitamins, float Carbs, float Water);
	void Server_AddNutrition_Implementation(float Protein, float Vitamins, float Carbs, float Water);
		
};
