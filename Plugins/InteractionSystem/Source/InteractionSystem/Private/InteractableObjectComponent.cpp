// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObjectComponent.h"
#include "PlayerInteractionSensor.h"

// Sets default values for this component's properties
UInteractableObjectComponent::UInteractableObjectComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UInteractableObjectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



