// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

void UMGGameInstance::loginEOS()
{
	//This is the online subsystem
	//This Gives us access to all of the online subsystem functions
	const IOnlineSubsystem* ossRef = Online::GetSubsystem(GetWorld());
	
	if (!ossRef)
		return;

	const IOnlineIdentityPtr identityRef = ossRef->GetIdentityInterface();

	if (!identityRef)
		return;
}
