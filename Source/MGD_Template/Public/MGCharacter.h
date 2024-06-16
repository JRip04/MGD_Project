// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MGCharacter.generated.h"

UCLASS()
class MGD_TEMPLATE_API AMGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMGCharacter();

	// Move Character based on an axis
	UFUNCTION(BlueprintCallable, Category="Movement")
	void Pure_MoveCharacter(const FVector2D Axis);

	//Rotate the character controller
	UFUNCTION(BlueprintCallable, Category="Controller")
	void Pure_RoatateController(const FVector2D Axis); 

public:
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	// Move axis of Character
	FVector2D pMoveAxis;
	
};
