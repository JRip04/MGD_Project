	// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/GameModeBase.h"
#include "Online/OnlineSessionNames.h"

	UMGGameInstance::UMGGameInstance()
	{
		FoundSessions = MakeShareable(new FOnlineSessionSearch());
		FoundSessions->MaxSearchResults = 10;
		FoundSessions->bIsLanQuery = true;
		FoundSessions->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	}

	void UMGGameInstance::Init()
	{
		Super::Init();

		//This is the online subsystem
		//This Gives us access to all the online subsystem functions
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

		//Binding the login complete function to the login complete in the identity interface
		identityRef->OnLoginCompleteDelegates->AddUObject(this, &UMGGameInstance::EosLoginComplete);

		const IOnlineSessionPtr sessionRef = ossRef->GetSessionInterface();

		if(!sessionRef)
			return;

		sessionRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UMGGameInstance::SessionCreateComplete);

		sessionRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UMGGameInstance::SessionFindComplete);

		sessionRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UMGGameInstance::SessionJoinComplete);
	}

void UMGGameInstance::loginEOS()
{
	//This is the online subsystem
	//This Gives us access to all the online subsystem functions
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
	// the account portal is a login portal that opens whe you try to log in
	FOnlineAccountCredentials accCreds;
	accCreds.Id = FString();
	accCreds.Token = FString();
	accCreds.Type = FString(TEXT("accountportal"));

	// Run the login portal
	identityRef->Login(0, accCreds);
}

	bool UMGGameInstance::IsLogedIn() const
	{
		//This is the online subsystem
		//This Gives us access to all the online subsystem functions
		const IOnlineSubsystem* ossRef = Online::GetSubsystem(GetWorld());

		//If the ossRef Returns Null
		// exit the function
		if (!ossRef)
			return false;

		// this is the identity interface for EOS
		// we can get info about the players accounts from this interface
		// it also allows us to access authentication (Login/Logout)
		const IOnlineIdentityPtr identityRef = ossRef->GetIdentityInterface();

		// if the identityRef returns null
		//exit the function
		if (!identityRef)
			return false;

		return identityRef->GetLoginStatus(0) == ELoginStatus::LoggedIn;
	}

	FString UMGGameInstance::GetDisplayName() const
	{
		if (!IsLogedIn())
			return FString(TEXT("DisplayName"));

		// Since is logged in already checks that the interface exists we can skip a step straight to the identity
		const IOnlineIdentityPtr identityRef = Online::GetSubsystem(GetWorld())->GetIdentityInterface();

		return identityRef->GetPlayerNickname(0);
	}

	bool UMGGameInstance::IsInSession() const
	{
		//Are we logged in
		//this checks if there is a online subsystem, so we can skip that check
		if(!IsLogedIn())
			return false;

		//get the interface that interacts with sessions, so we can get session data
		const IOnlineSessionPtr SessionRef = Online::GetSubsystem(GetWorld())->GetSessionInterface();

		//make sure there is a session interface
		if (!SessionRef)
			return false;

		// get and locally store the session state
		const EOnlineSessionState::Type state = SessionRef->GetSessionState(MGSESSION_NAME);

		//if there is a session return true
		return state != EOnlineSessionState::NoSession;
	}

	void UMGGameInstance::HostGame(bool lan)
	{
		if (!IsLogedIn())
			return;

		const IOnlineSessionPtr sessionRef = Online::GetSubsystem(GetWorld())->GetSessionInterface();

		if (!sessionRef)
			return;

		FOnlineSessionSettings settings;
		settings.NumPublicConnections = 4;
		settings.bIsLANMatch = lan;
		settings.bIsDedicated = false;
		settings.bAllowInvites = true;
		settings.bShouldAdvertise = true;
		settings.bUsesPresence = true;
		settings.bUseLobbiesIfAvailable = true;
		settings.Set(SEARCH_KEYWORDS, MGSESSION_NAME.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
		settings.Set(SEARCH_LOBBIES, true, EOnlineDataAdvertisementType::ViaOnlineService);

		sessionRef->CreateSession(0,MGSESSION_NAME, settings);
	}

	void UMGGameInstance::FindAndJoinSession()
	{
		if(!IsLogedIn())
		return;

		const IOnlineSessionPtr sessionRef = Online::GetSubsystem(GetWorld())->GetSessionInterface();

		if(!sessionRef)
			return;

		UE_LOG(LogTemp, Warning, TEXT("Finding sessions"))
		sessionRef->FindSessions(0, FoundSessions.ToSharedRef());
	}

	void UMGGameInstance::StartLobbyGame()
	{
		GetWorld()->GetAuthGameMode()->bUseSeamlessTravel = true;
		GetWorld()->ServerTravel("/Game/MyContent/Maps/Lvl_Test", false);
	}

	void UMGGameInstance::EosLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
	                                       const FString& Error)
	{
		OnLoginComplete(bWasSuccessful, Error);
	}

	void UMGGameInstance::SessionCreateComplete(FName SessionName, bool bWasSuccessful)
	{
		EnableListenServer(true);
		OnSessionCreateComplete(bWasSuccessful);
	}

	void UMGGameInstance::SessionFindComplete(bool bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Searching sessions complete"))
		
		if(!bWasSuccessful)
		{
			UE_LOG(LogTemp, Error, TEXT("FAILED TO FIND SESSION"))
			OnSessionJoinComplete(false);
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("Getting oss"))
		const IOnlineSubsystem* ossRef = Online::GetSubsystem(GetWorld());

		if(!ossRef)
			return;

		UE_LOG(LogTemp, Warning, TEXT("Getting session ref"))
		const IOnlineSessionPtr sessionRef = ossRef->GetSessionInterface();

		if (!sessionRef)
			return;

		UE_LOG(LogTemp, Warning, TEXT("Searching through results"))
		if(FoundSessions->SearchResults.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("NO SESSIONS FOUND"))
			OnSessionJoinComplete(false);
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("FOUND SESSIONS, ATTEMPTING TO JOIN"))

		//Join the sessions that is the first session found
		sessionRef->JoinSession(0, MGSESSION_NAME, FoundSessions->SearchResults[0]);
	}

	void UMGGameInstance::SessionJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
	{
		OnSessionJoinComplete(Result == EOnJoinSessionCompleteResult::Success);
		
		if(Result != EOnJoinSessionCompleteResult::Success)
		{
			UE_LOG(LogTemp, Error, TEXT("FAILED TO JOIN SESSION"))
			return;
		}

		ClientTravelToSession(0, SessionName);
	}

