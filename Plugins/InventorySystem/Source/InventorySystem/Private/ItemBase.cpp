// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

//UE4 Includes
#include "Net/UnrealNetwork.h"

// Sets default values
AItemBase::AItemBase()
{

	SetReplicates(true);

}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, ItemData);


}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::UpdateItemInformation(FItemData NewItemData)
{
	ItemData = NewItemData;
}



