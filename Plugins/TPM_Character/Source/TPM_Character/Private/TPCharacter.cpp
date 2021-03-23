// Fill out your copyright notice in the Description page of Project Settings.


#include "TPCharacter.h"

//Custom Invludes
#include "TPCharacterMovementComponent.h"
#include "TPM_Character.h"

//UE4 Includes
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ATPCharacter::ATPCharacter(const FObjectInitializer& ObjectInitializer)


	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))

	{

		// ==== Default Values ===== =====
		PrimaryActorTick.bCanEverTick = true;
		bReplicates = true;

		// ==== Components ===== =====

		//Add Spring Arm for Camera
		SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
		SpringArmComp->SetupAttachment(RootComponent);
		SpringArmComp->bUsePawnControlRotation = false;
		SpringArmComp->bEnableCameraLag = true;
		SpringArmComp->bEnableCameraRotationLag = true;
		SpringArmComp->bInheritPitch = false;

		//Spring Arm default values
		SpringArmComp->SetRelativeLocation(FVector(20.f, 0.f, 85.f));
		SpringArmComp->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
		SpringArmComp->TargetArmLength = 150.f;

		//Add Camera Component
		CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
		CameraComp->SetupAttachment(SpringArmComp);


		//Default Movement Values
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
		GetCharacterMovement()->MaxWalkSpeedCrouched = DefaultCrouchSpeed;


	}


// Called when the game starts or when spawned
void ATPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ==== Replicated Variables ===== =====

void ATPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPCharacter, bWantsToSprint);
	DOREPLIFETIME(ATPCharacter, bIsSprinting);	
	DOREPLIFETIME(ATPCharacter, SprintSpeedMuliplyer)
	

}




// ==== Movement ===== =====


// Called to bind functionality to input
void ATPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &ATPCharacter::TurnRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPCharacter::LookUp);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPCharacter::Jump);
	PlayerInputComponent->BindAction("ToggleCrouch", IE_Pressed, this, &ATPCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("ToggleSprint", IE_Pressed, this, &ATPCharacter::SetWantsToSprint);

}

bool ATPCharacter::GetWantsToSprint() 
{
	// resets sprint if not moving
	if (GetVelocity().Size() == 0 && bWantsToSprint == true)
	{
		SetWantsToSprint(); 
	}

	return bWantsToSprint;
}

float ATPCharacter::GetSprintSpeedModifier() {return SprintSpeedMuliplyer;}

void ATPCharacter::SetSprintSpeedModifer(float NewSprintModifer)
{
	if (HasAuthority())
	{
		SprintSpeedMuliplyer = NewSprintModifer;
	}

}

float ATPCharacter::GetDefaultWalkSpeed() {return DefaultWalkSpeed;}


//Forward and Back
void ATPCharacter::MoveForward(float Value)
{
	
	AddMovementInput(GetActorForwardVector() * Value);

}


//Right and Left
void ATPCharacter::MoveRight(float Value)
{

	AddMovementInput(GetActorRightVector() * Value);

}

void ATPCharacter::TurnRight(float Value)
{

	AddControllerYawInput(Value);

}


void ATPCharacter::LookUp(float Value)
{
	
	if (CameraComp == nullptr) { return; }

	AddControllerPitchInput(Value);


}

void ATPCharacter::Jump()
{
	Super::Jump();
}

void ATPCharacter::ToggleCrouch()
{
	if (GetCharacterMovement()->IsCrouching())
	{
		UnCrouch();
	}
	else
	{
		Crouch();
		if (bWantsToSprint == true) {SetWantsToSprint();}
	}
}



void ATPCharacter::SetWantsToSprint()
{
	
	if (GetLocalRole() < ROLE_Authority)
	{
		// Is NOT Authority
		Server_SetWantsToSprint();
	}
	else
	{
		//Is Authority
		if (bWantsToSprint == false)
		{
			bWantsToSprint = true;
			WantsToSprint_OnSprintStart.Broadcast();
			UE_LOG(LogTPCharacter, Log, TEXT("Character wants to sprint"))
			
		}
		else
		{
			bWantsToSprint = false;
			WantsToNotSprint_OnSptrintStop.Broadcast();
			UE_LOG(LogTPCharacter, Log, TEXT("Character wants to stop sprint"))
		}
	}
}


bool ATPCharacter::Server_SetWantsToSprint_Validate()
{
	return true;
}

void ATPCharacter::Server_SetWantsToSprint_Implementation()
{
	SetWantsToSprint();

}

float ATPCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage_DamageTaken.Broadcast(DamageAmount);

	return DamageAmount;
}



