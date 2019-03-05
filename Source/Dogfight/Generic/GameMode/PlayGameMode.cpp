// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "PlayGameMode.h"
#include "Generic/PoolingManager.h"

#include "InGameActors/PlaneCharacter.h"
#include "Generic/DogfighterPlayerController.h"
#include "Generic/PlayerHUD.h"
#include "Generic/GameInfoInstance.h"

APlayGameMode::APlayGameMode()
{
	
}

void APlayGameMode::BeginPlay()
{
	PoolingManager = (APoolingManager*)GetWorld()->SpawnActor(APoolingManager::StaticClass());

	UGameInfoInstance* GI = Cast<UGameInfoInstance>(GetGameInstance());

	if (GI != nullptr)
	{
		MaxPlayers = GI->MaxPlayers;
		ServerName = GI->ServerName;
	}
}

APoolingManager* APlayGameMode::GetPoolingManager()
{
	APoolingManager* Out = PoolingManager;

	return Out;
}

int APlayGameMode::UpdateScore(ADogfighterPlayerController* Player, int ScoreUpdate)
{
	int FinalScore = 0;

	if (Player != nullptr)
	{
		FPlayerInformation PlayerInfo = Player->GetPlayerInformation();

		int* PlayerScore = Scores.Find(PlayerInfo.PlayerName);

		FinalScore = (PlayerScore != nullptr ? *PlayerScore + ScoreUpdate : ScoreUpdate);

		Scores.Add(PlayerInfo.PlayerName,  FinalScore);
	}

	ReorderLeaderboard();

	return FinalScore;
}

void APlayGameMode::ReorderLeaderboard()
{
	TArray<int> ScoreValues;
	TArray<FString> ScoreKeys, Tied;
	Scores.GetKeys(ScoreKeys);
	int* Temp;

	for (FString Key : ScoreKeys)
	{
		Temp = Scores.Find(Key);

		if (Temp != nullptr)
		{
			ScoreValues.AddUnique(*Temp);
		}
	}

	//Sort Values
	Scores.ValueSort([](const int& A, const int& B)
	{
		return (A > B);
	});

	//Store the currently sorted keys as the new leaderboard.
	Scores.GetKeys(Leaderboard);

	//TODO:
	//Replicate Scores + Leaderboard.
}

void APlayGameMode::Server_RequestRespawn_Implementation(AController * Player)
{
	AActor* NewSpawn = ChoosePlayerStart(Player);
	
	APlaneCharacter* Plane = Cast<APlaneCharacter>(Player->GetPawn());

	if (Plane != nullptr && NewSpawn != nullptr)
	{
		Plane->Server_OnPlayerRespawn(NewSpawn->GetTransform());
		
		ADogfighterPlayerController* Controller = Cast<ADogfighterPlayerController>(Player);
		
		if (Controller != nullptr)
		{
			Controller->OnPlayerRespawned();
		}
	}
}
bool APlayGameMode::Server_RequestRespawn_Validate(AController * Player) { return true; }

void APlayGameMode::GetRespawnTimer(AController * Dead, float & OutMinTime, float & OutMaxTime)
{
	OutMinTime = MinRespawnDelay;
	OutMaxTime = MaxRespawnTime;
}

void APlayGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ADogfighterPlayerController* Controller = Cast<ADogfighterPlayerController>(NewPlayer);

	if (Controller != nullptr)
	{
		//Get the player's information
		FPlayerInformation PlayerInfo = Controller->GetPlayerInformation();

		TArray<ADogfighterPlayerController*> Oldcomers;
		PlayersInServer.GetKeys(Oldcomers);

		for (ADogfighterPlayerController* OldComer : Oldcomers)
		{
			//Update Oldcomer's screen with New Player.
			OldComer->AddNewPlayerToGameLobbyScreen(PlayerInfo);

			FPlayerInformation* OldComerInfo = PlayersInServer.Find(OldComer);
			if (OldComerInfo != nullptr)
			{
				//Update Newcomer's screen with current Oldcomer.
				Controller->AddNewPlayerToGameLobbyScreen(*OldComerInfo);
			}

		}

		//Add the player to the list on the server.
		PlayersInServer.Add(Controller, PlayerInfo);

		//Tell the PlayerController to update its HUD about the connection.
		Controller->bShowMouseCursor = true;
		Controller->ShowGameLobbyScreen();
		Controller->AddNewPlayerToGameLobbyScreen(PlayerInfo);
		Controller->Server_ReceiveServerName(ServerName);
	}

	if (MaxPlayers == PlayersInServer.Num())
	{
		StartMatch();
	}
}

void APlayGameMode::StartMatch()
{
	Super::StartMatch();

	TArray<ADogfighterPlayerController*> Players;
	PlayersInServer.GetKeys(Players);

	for (ADogfighterPlayerController* Player : Players)
	{
		RestartPlayer((AController*)Player);
		Server_RequestRespawn(Player);
		Player->HideGameLobbyScreen();
		Player->ShowInGameInterface();
		Player->bShowMouseCursor = false;
	}
}

void APlayGameMode::EndMatch()
{
	Super::EndMatch();

	UE_LOG(LogTemp, Warning, TEXT("Game Over"));

	TArray<ADogfighterPlayerController*> Players;
	PlayersInServer.GetKeys(Players);

	for (ADogfighterPlayerController* Player : Players){
		if (Player != nullptr)
		{
			Player->Server_EndOfRound();
		}
	}

	GetWorldTimerManager().SetTimer(EndOfMatchTimer, this, &APlayGameMode::EndOfMatchTimerFinished, EndOfMatchWaitDelay, false);
}

void APlayGameMode::EndOfMatchTimerFinished()
{
	TArray<ADogfighterPlayerController*> Players;
	PlayersInServer.GetKeys(Players);

	for (ADogfighterPlayerController* Player : Players)
	{
		if (Player != nullptr)
		{
			Player->ShowGameLobbyScreen();
		}
	}
}

void APlayGameMode::Server_UpdatePlayerInformation_Implementation(AController* Player, FPlayerInformation PlayerInfo)
{
	ADogfighterPlayerController* Controller = Cast<ADogfighterPlayerController>(Player);

	if (PlayersInServer.Contains(Controller))
	{
		PlayersInServer.Add(Controller,PlayerInfo);

		TArray<ADogfighterPlayerController*> Players;
		PlayersInServer.GetKeys(Players);

		for (ADogfighterPlayerController* CurrentPlayer : Players)
		{
			if (CurrentPlayer != nullptr)
			{
				CurrentPlayer->UpdatePlayerInformation(PlayerInfo);
			}
		}
	}
}

bool APlayGameMode::Server_UpdatePlayerInformation_Validate(AController* Player, FPlayerInformation PlayerInfo)
{
	return true;
}

//If Killer is a Player, + 1 point to Killer.
//If Killer is else, Dead -1 point.
void APlayGameMode::OnPlayerDied_Implementation(AController * Dead, AActor * Killer)
{
	bool bSuicide = false;
	ADogfighterPlayerController* PlayerKiller;

	//Cast Killer to check if it is a player.
	PlayerKiller = Cast<ADogfighterPlayerController>(Killer);

	if (PlayerKiller != nullptr)
	{
		if (PlayerKiller == Cast<ADogfighterPlayerController>(Dead))
		{
			bSuicide = true;
		}
	}

	if (!bSuicide && PlayerKiller == nullptr)
	{
		//Check if Killer is someone's pawn.
		APlaneCharacter* Plane = Cast<APlaneCharacter>(Killer);

		if (Plane != nullptr)
		{
			PlayerKiller = Cast<ADogfighterPlayerController>(Plane->GetController());
		}
	}

	//Make sure it wasn't a suicide.
	if (!bSuicide)
	{
		UpdateScore(PlayerKiller, 1);
	}
	else
	{
		ADogfighterPlayerController* PlayerDead = Cast<ADogfighterPlayerController>(Dead);

		UpdateScore(PlayerDead, -1);
	}

}
bool APlayGameMode::OnPlayerDied_Validate(AController * Dead, AActor * Killer)
{
	return true;
}