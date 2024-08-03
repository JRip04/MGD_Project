// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameMode_Lobby.h"

#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AMGGameMode_Lobby::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	//Initialise an empty local array of player starts
	TArray<AActor*> PlayerStartArray;

	//Get All of Player starts and add them into the PlayerStartArray variable
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(),PlayerStartArray);

	//if there are no player starts then stop the function
	if (PlayerStartArray.IsEmpty())
		return;
	
    // looping through all of the actors in the array
	for (AActor* ActorRef : PlayerStartArray)
	{
		//Testing to make sure the actor is a player start
		if (APlayerStart* psRef = Cast<APlayerStart>(ActorRef))
		{
			//assigning a index depending on the tag for the player start
			int PlayerStartIndex = 1;

			if (psRef->PlayerStartTag == FName("Player2"))
			{
				PlayerStartIndex = 2;
			}
			else if(psRef->PlayerStartTag == FName("Player3"))
			{
				PlayerStartIndex = 3;
			}
			else if (psRef->PlayerStartTag == FName("Player4"))
			{
				PlayerStartIndex = 4;
			}
			//getting the number of players in the game
			int32 numPlayers = UGameplayStatics::GetNumPlayerStates(GetWorld());

			// Checking if the player start tag matches this player
			if (PlayerStartIndex == numPlayers)
			{

				// Restart the player and add it to the correct player start
				RestartPlayerAtPlayerStart(NewPlayer, psRef);
				break;
			}
		}
	}
}
