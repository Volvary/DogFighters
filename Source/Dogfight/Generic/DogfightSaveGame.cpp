// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "DogfightSaveGame.h"
#include "Dogfight.h"

UDogfightSaveGame::UDogfightSaveGame()
{

}

void UDogfightSaveGame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(UDogfightSaveGame, PlayerInfo);
}