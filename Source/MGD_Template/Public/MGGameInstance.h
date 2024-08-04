// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MGD_TEMPLATE_API UMGGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// Allows other Classes to Use that function
public:
	void loginEOS();

	// Only this class and Classes Derived can use this function
protected:
	
	// Only this class can access these functions
private:
	
};
