// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "GameLobbyScreen.h"

void UGameLobbyScreen::RemovePlayerFromList_Implementation(FPlayerInformation RemovedPlayer)
{

}

void UGameLobbyScreen::NewPlayerJoined_Implementation(FPlayerInformation NewPlayer)
{

}

void UGameLobbyScreen::Replicate_RemovePlayerFromList_Implementation(FPlayerInformation RemovedPlayer)
{
	RemovePlayerFromList(RemovedPlayer);
}

void UGameLobbyScreen::Replicate_NewPlayerJoined_Implementation(FPlayerInformation NewPlayer)
{
	NewPlayerJoined(NewPlayer);
}

void UGameLobbyScreen::UpdatePlayerInformation_Implementation(FPlayerInformation PlayerUpdated)
{

}

void UGameLobbyScreen::Replicate_UpdatePlayerInformation_Implementation(FPlayerInformation NewPlayer)
{
	UpdatePlayerInformation(NewPlayer);
}

void UGameLobbyScreen::ReceiveServerName_Implementation(const FText& InServerName)
{

}
