// Fill out your copyright notice in the Description page of Project Settings.


#include "MGCharacter.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMGCharacter::AMGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMGCharacter::Pure_MoveCharacter(const FVector2D Axis)
{
	// update the member axis
	pMoveAxis = Axis;

	//Move Character based on Y axis
	AddMovementInput( GetControlRotation().Vector(),Axis.Y);

	//Move Character Based on X axis
	AddMovementInput(UKismetMathLibrary:: GetRightVector(GetControlRotation()), Axis.X);
}

void AMGCharacter::Pure_RoatateController(const FVector2D Axis)
{
	//Rotate Controller Yaw based on X axis
	AddControllerYawInput(Axis.X);

	//Rotate the controller Pitch based on Y axis
	AddControllerPitchInput(-Axis.Y);
}


