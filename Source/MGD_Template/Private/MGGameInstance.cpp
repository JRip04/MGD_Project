	// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

	void UMGGameInstance::Init()
	{
		Super::Init();

		//This is the online subsystem
		//This Gives us access to all of the online subsystem functions
		const IOnlineSubsystem* ossRef = Online::GetSubsystem(GetWorld());

		//If the ossRef Returns Null
		// exit the function
		if (!ossRef)
			return;

		// this is the identity interface for EOS
		// we can get info about the players accounts from this interface
		// it also allows us to access authentication (Login/Logout)
		const IOnlineIdentityPtr identityRef = ossRef->GetIdentityInterface();

		// if the identityRef returns null
		//exit the function
		if (!identityRef)
			return;
	}

void UMGGameInstance::loginEOS()
{
	//This is the online subsystem
	//This Gives us access to all of the online subsystem functions
	const IOnlineSubsystem* ossRef = Online::GetSubsystem(GetWorld());

	//If the ossRef Returns Null
	// exit the function
	if (!ossRef)
		return;

	// this is the identity interface for EOS
	// we can get info about the players accounts from this interface
	// it also allows us to access authentication (Login/Logout)
	const IOnlineIdentityPtr identityRef = ossRef->GetIdentityInterface();

	// if the identityRef returns null
	//exit the function
	if (!identityRef)
		return;

	// this sets up the type of account credentials that the login will expect
	// the account portal is a login portal that opens whe you try to login
	FOnlineAccountCredentials accCreds;
	accCreds.Id = FString();
	accCreds.Token = FString();
	accCreds.Type = FString(TEXT("accountportal"));

	// Run the login portal
	identityRef->Login(0, accCreds);
}
