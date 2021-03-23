// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInteractionSensor.h"
#include "InteractableObjectComponent.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerInteractionSensor::UPlayerInteractionSensor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);

	// ...
}


// Called when the game starts
void UPlayerInteractionSensor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPlayerInteractionSensor::TriggerInteraction(UInteractableObjectComponent* InteractableComponent)
{
	//If one of the components is not valid, exit function
	if (!InteractableComponent) { return; }

	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_TriggerInteraction(InteractableComponent);
	}
	else
	{

		InteractableComponent->InteractionTriggered(this);

	}


}

bool UPlayerInteractionSensor::Server_TriggerInteraction_Validate(UInteractableObjectComponent* InteractableComponent)
{
	return true;
}

void UPlayerInteractionSensor::Server_TriggerInteraction_Implementation(UInteractableObjectComponent* InteractableComponent)
{
	TriggerInteraction(InteractableComponent);
}


