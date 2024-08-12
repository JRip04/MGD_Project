// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MGGameInstance.generated.h"


#define MGSESSION_NAME FName(TEXT("MGSESSION"))

/**
 * 
 */
UCLASS()
class MGD_TEMPLATE_API UMGGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// Allows other Classes to Use that function
public:
	UMGGameInstance();
	
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category="Login")
	void loginEOS();

	UFUNCTION(BlueprintPure, Category="login")
	bool IsLogedIn() const;

	UFUNCTION(BlueprintPure, Category="User")
	FString GetDisplayName() const;

	UFUNCTION(BlueprintPure, Category="Session")
	bool IsInSession() const;

	UFUNCTION(BlueprintCallable, Category="Session")
	void HostGame(bool lan);

	UFUNCTION(BlueprintCallable, Category="Session")
	void FindAndJoinSession();

//Start a lobby game if one exists
	UFUNCTION(BlueprintCallable, Category="Session")
	void StartLobbyGame();
	

	// Only this class and Classes Derived can use this function
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Login")
	void OnLoginComplete(bool success, const FString& Error);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Session")
	void OnSessionCreateComplete(bool success);

	UFUNCTION(BlueprintImplementableEvent,Category="Session")
	void OnSessionJoinComplete(bool success);

	// Only this class can access these functions
private:
	//Runs When Eos Login Completes
	void EosLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	void SessionCreateComplete(FName SessionName, bool bWasSuccessful);

	void SessionFindComplete(bool bWasSuccessful);

	void SessionJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	TSharedPtr<FOnlineSessionSearch> FoundSessions;
};
