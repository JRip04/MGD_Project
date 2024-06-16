// Fill out your copyright notice in the Description page of Project Settings.


#include "MGCharacter.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMGCharacter::AMGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pRepPitch = 0.0f;

}

void AMGCharacter::Pure_MoveCharacter(const FVector2D Axis)
{
	// update the member axis
	pMoveAxis = Axis;

	//Move Character based on Y axis
	AddMovementInput( GetActorRotation().Vector(),Axis.Y);

	//Move Character Based on X axis
	AddMovementInput(UKismetMathLibrary:: GetRightVector(GetActorRotation()), Axis.X);
}

void AMGCharacter::Pure_RotateController(const FVector2D Axis)
{
	//Rotate Controller Yaw based on X axis
	AddControllerYawInput(Axis.X);

	//Rotate the controller Pitch based on Y axis
	AddControllerPitchInput(-Axis.Y);

	//is the pRepPitch 10 units Difference from the Controllers Pitch
	if (FMath::Abs(pRepPitch - GetControlRotation().Pitch) > 10.0f)
	{
		//are we the server
		if ( HasAuthority())
		{
			Multi_RepPitch(GetControlRotation().Pitch);
		}
		else
		{
			Server_RepPitch(GetControlRotation().Pitch);
		}
			
	}
}

void AMGCharacter::Server_RepPitch_Implementation(const float& Pitch)
{
	//Run the MultiCast to update all the Clients
	Multi_RepPitch(Pitch);
}

void AMGCharacter::Multi_RepPitch_Implementation(const float& Pitch)
{
	//Update the pitch to match the value
	pRepPitch = Pitch;
}


