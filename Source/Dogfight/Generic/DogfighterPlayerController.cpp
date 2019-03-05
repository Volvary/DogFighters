// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "DogfighterPlayerController.h"
#include "InGameActors/PlaneCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Generic/GameMode/PlayGameMode.h"
#include "Dogfight.h"

#include "Generic/PlayerHUD.h"
#include "UI/RespawnScreen.h"
#include "UI/ScoreBoard.h"
#include "Generic/GameInfoInstance.h"

void ADogfighterPlayerController::ControlledCharacterDied(APlaneCharacter* Dead, AActor* Killer)
{
	if (HasAuthority())
	{
		APlayGameMode* GM = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());

		//Enable Inputs to use Respawn.
		Client_ToggleInput(true);

		float MinTime = 0.0f;
		float MaxTime = 0.0f;

		if (GM != nullptr)
		{
			GM->OnPlayerDied(this, Dead);

			GM->GetRespawnTimer(this, MinTime, MaxTime);
			Server_SetRespawnTimer(MinTime, MaxTime);
		}

		CheckHUDReference();

		if (PlayerHUD != nullptr)
		{
			PlayerHUD->ValidatedGetRespawnScreen()->SetRemainingTime(MinTime);
			SecondsUntilRespawn = MinTime;
		}
	}
}

void ADogfighterPlayerController::HideScoreBoard_Implementation()
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->HideScoreBoard();
	}
}

void ADogfighterPlayerController::ShowScoreBoard_Implementation()
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->ShowScoreBoard();
	}
}

void ADogfighterPlayerController::UpdateHealtRemaining_Implementation(float InHealthRatio)
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->UpdateHealthLeft(InHealthRatio);
	}
}

void ADogfighterPlayerController::HideInGameInterface_Implementation()
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->HideInGameInterface();
	}
}

void ADogfighterPlayerController::ShowInGameInterface_Implementation()
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->ShowInGameInterface();
	}
}

void ADogfighterPlayerController::UpdatePlayerInformation_Implementation(FPlayerInformation PlayerInfo)
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->UpdatePlayerInformation(PlayerInfo);
	}
}

void ADogfighterPlayerController::ShowGameLobbyScreen_Implementation()
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->ShowGameLobbyScreen();
	}
}

void ADogfighterPlayerController::HideGameLobbyScreen_Implementation()
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->HideGameLobbyScreen();
	}
}

void ADogfighterPlayerController::AddNewPlayerToGameLobbyScreen_Implementation(FPlayerInformation NewPlayer)
{
	CheckHUDReference();
	
	if (PlayerHUD != nullptr)
	{
		PlayerHUD->AddNewPlayerToGameLobbyScreen(NewPlayer);
	}
}

void ADogfighterPlayerController::RequestRespawn()
{
	if (PlanePawn == nullptr)
	{
		PlanePawn = Cast<APlaneCharacter>(GetPawn());
	}

	if (PlanePawn != nullptr && bCanRequestRespawn)
	{
		Server_RequestRespawn();
	}
}

void ADogfighterPlayerController::OnPlayerRespawned_Implementation()
{
	Client_ToggleInput(false);

	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->ValidatedGetRespawnScreen()->SetVisibility(ESlateVisibility::Hidden);
		PlayerHUD->UpdateHealthLeft(1.0f);
	}
}

void ADogfighterPlayerController::Server_RequestRespawn_Implementation()
{
	//Confirm player is not cheating or desynchronized.
	if (bCanRequestRespawn)
	{
		Server_SetCanRespawn(false);
		GetWorldTimerManager().ClearTimer(MinRespawnTimeHandle);
		GetWorldTimerManager().ClearTimer(MaxRespawnTimeHandle);


		if (HasAuthority())
		{
			APlayGameMode* GM = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());

			if (GM != nullptr)
			{
				GM->Server_RequestRespawn(this);
			}
		}
	}
}
bool ADogfighterPlayerController::Server_RequestRespawn_Validate(){ return true; }

void ADogfighterPlayerController::Server_SetRespawnTimer_Implementation(float MinTime, float MaxTime)
{
	GetWorldTimerManager().SetTimer(MinRespawnTimeHandle, this, &ADogfighterPlayerController::Server_RespawnTimerDone, MinTime, false);
	GetWorldTimerManager().SetTimer(MaxRespawnTimeHandle, this, &ADogfighterPlayerController::Server_RequestRespawn, MaxTime, false);

	//Notify the Player
	Replicate_SetTimeUntilRespawn(MinTime);
}
bool ADogfighterPlayerController::Server_SetRespawnTimer_Validate(float MinTime, float MaxTime){ return true; }

void ADogfighterPlayerController::Server_SetCanRespawn_Implementation(bool CanRespawn)
{
	bCanRequestRespawn = CanRespawn;
	Client_SetCanRespawn(CanRespawn);
}
bool ADogfighterPlayerController::Server_SetCanRespawn_Validate(bool CanRespawn) { return true; }

void ADogfighterPlayerController::Client_SetCanRespawn_Implementation(bool CanRespawn)
{
	CheckHUDReference();

	PlayerHUD->ValidatedGetRespawnScreen()->SetCanRespawn(CanRespawn);
}

void ADogfighterPlayerController::Server_RespawnTimerDone_Implementation()
{
	Server_SetCanRespawn(true);
}
bool ADogfighterPlayerController::Server_RespawnTimerDone_Validate(){ return true; }

void ADogfighterPlayerController::Replicate_SetTimeUntilRespawn_Implementation(float MinTimeUntilRespawn)
{
	int TimeRemaining = FMath::CeilToInt(MinTimeUntilRespawn);

	SecondsUntilRespawn = TimeRemaining;

	GetWorldTimerManager().SetTimer(VisualTimerHandle, this, &ADogfighterPlayerController::UpdateSecondsUntilRespawn, 1.0f, true, MinTimeUntilRespawn - FMath::FloorToInt(MinTimeUntilRespawn));
}

void ADogfighterPlayerController::UpdateSecondsUntilRespawn_Implementation()
{
	SecondsUntilRespawn -= 1;

	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->ValidatedGetRespawnScreen()->SetRemainingTime(SecondsUntilRespawn);
		PlayerHUD->ValidatedGetRespawnScreen()->SetVisibility(ESlateVisibility::Visible);
	}

	if (SecondsUntilRespawn <= 0)
	{
		GetWorldTimerManager().ClearTimer(VisualTimerHandle);
	}
}

void ADogfighterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Respawn", IE_Pressed, this, &ADogfighterPlayerController::RequestRespawn);

	Client_ToggleInput(false);
}

void ADogfighterPlayerController::CheckHUDReference()
{
	if (PlayerHUD == nullptr)
	{
		PlayerHUD = Cast<APlayerHUD>(GetHUD());
	}
}

void ADogfighterPlayerController::Client_ToggleInput_Implementation(bool Active)
{
	if (Active)
	{
		EnableInput(this);
	}
	else
	{
		DisableInput(this);
	}
}

void ADogfighterPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Here we list the variables we want to replicate + a condition if wanted
	DOREPLIFETIME(ADogfighterPlayerController, bCanRequestRespawn);
}

FPlayerInformation ADogfighterPlayerController::GetPlayerInformation()
{
	if (HasAuthority())
	{
		Client_RequestPlayerInfo();
	}

	return PlayerInformation;
}

void ADogfighterPlayerController::Client_EndOfRound_Implementation()
{
	ShowScoreBoard();
	HideInGameInterface();
	
	URespawnScreen* Screen = PlayerHUD->ValidatedGetRespawnScreen();

	if (Screen != nullptr)
	{
		Screen->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ADogfighterPlayerController::Server_EndOfRound_Implementation()
{
	PlanePawn = Cast<APlaneCharacter>(GetPawn());

	if (PlanePawn != nullptr)
	{
		PlanePawn->Destroy();
	}

	Client_EndOfRound();
}
bool ADogfighterPlayerController::Server_EndOfRound_Validate(){	return true;}

void ADogfighterPlayerController::Client_PopulateScoreBoard_Implementation(const TArray<FPlayerScore>& LoggedPlayers)
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		UScoreBoard* ScoreBoard = PlayerHUD->ValidatedGetScoreBoard();

		if (ScoreBoard == nullptr)
		{

			ScoreBoard->BulkUpdateBoard(LoggedPlayers, true);
		}
	}
}

void ADogfighterPlayerController::Client_ReceiveTimeLeftUpdate_Implementation(const int InMatchTimeRemaining)
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->UpdateServerTimeLeft(InMatchTimeRemaining);
	}
}

void ADogfighterPlayerController::Server_ReceiveTimeLeftUpdate_Implementation(const int InMatchTimeRemaining)
{
	Client_ReceiveTimeLeftUpdate(InMatchTimeRemaining);
}
bool ADogfighterPlayerController::Server_ReceiveTimeLeftUpdate_Validate(const int InMatchTimeRemaining){return true;}

void ADogfighterPlayerController::Server_ReceiveScoreUpdate_Implementation(const TArray<FPlayerScore>& InLeaderboard)
{
	//TODO: Transmit to HUD;
}

bool ADogfighterPlayerController::Server_ReceiveScoreUpdate_Validate(const TArray<FPlayerScore>& InLeaderboard)
{
	return true;
}

void ADogfighterPlayerController::Client_ReceiveServerName_Implementation(const FText& ServerName)
{
	CheckHUDReference();

	if (PlayerHUD != nullptr)
	{
		PlayerHUD->ReceiveServerName(ServerName);
	}
}

void ADogfighterPlayerController::Server_ReceiveServerName_Implementation(const FText& ServerName)
{
	Client_ReceiveServerName(ServerName);
}
bool ADogfighterPlayerController::Server_ReceiveServerName_Validate(const FText& ServerName)
{
	return true;
}

void ADogfighterPlayerController::Server_ReceivePlayerInfo_Implementation(FPlayerInformation PlayerInfo)
{
	PlayerInformation = PlayerInfo;

	APlayGameMode* GM = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	if (GM != nullptr)
	{
		GM->Server_UpdatePlayerInformation(this, PlayerInfo);
	}
} 
bool ADogfighterPlayerController::Server_ReceivePlayerInfo_Validate(FPlayerInformation PlayerInfo){ return true;}

void ADogfighterPlayerController::Client_RequestPlayerInfo_Implementation()
{

	if (!bObtainedPlayerInformation)
	{
		bObtainedPlayerInformation = true;
		UGameInfoInstance* GI = Cast<UGameInfoInstance>(GetGameInstance());
		if (GI != nullptr)
		{
			PlayerInformation = GI->GetPlayerData();
		}
	}
	
	Server_ReceivePlayerInfo(PlayerInformation);
}
