// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "PlayerHUD.h"
#include "UI/MainMenuScreen.h"
#include "UI/RespawnScreen.h"
#include "UI/FirstConnectionScreen.h"
#include "UI/JoinGameScreen.h"
#include "UI/HostGameScreen.h"
#include "UI/GameLobbyScreen.h"
#include "UI/InGameInterface.h"
#include "UI/ScoreBoard.h"

void APlayerHUD::BeginPlay()
{

}

URespawnScreen* APlayerHUD::ValidatedGetRespawnScreen()
{
	URespawnScreen* OutScreen = RespawnScreen;
	
	if (OutScreen == nullptr)
	{
		RespawnScreen = (URespawnScreen*)CreateWidget<URespawnScreen>(GetGameInstance(), RespawnScreenClass);
		RespawnScreen->AddToViewport(1);
		OutScreen = RespawnScreen;
	}

	return OutScreen;
}

UScoreBoard* APlayerHUD::ValidatedGetScoreBoard(bool bShowIfCreated)
{
	if (ScoreBoardScreen == nullptr)
	{
		//TODO: Replace with a CreateWidget in a proper HUD Manager.
		ShowScoreBoard();
		
		if(!bShowIfCreated){
			HideScoreBoard();
		}
	}
	
	return ScoreBoardScreen;
}

void APlayerHUD::AddNewPlayerToGameLobbyScreen_Implementation(FPlayerInformation NewPlayer)
{
	if (GameLobbyScreen != nullptr)
	{
		GameLobbyScreen->NewPlayerJoined(NewPlayer);
	}
}

void APlayerHUD::RemovePlayerFromGameLobbyScreen_Implementation(FPlayerInformation RemovedPlayer)
{
	if (GameLobbyScreen != nullptr)
	{
		GameLobbyScreen->RemovePlayerFromList(RemovedPlayer);
	}
}

void APlayerHUD::UpdatePlayerInformation_Implementation(FPlayerInformation RemovedPlayer)
{
	if(GameLobbyScreen != nullptr)
	{
		GameLobbyScreen->UpdatePlayerInformation(RemovedPlayer);
	}
}

void APlayerHUD::ReceiveServerName(const FText& InServerName)
{
	if (GameLobbyScreen != nullptr)
	{
		GameLobbyScreen->ReceiveServerName(InServerName);
	}
}

void APlayerHUD::UpdateServerTimeLeft(const int InMatchTimeLeft)
{
	//TODO: Uncomment
	//if(InGameUI != nullptr){
	//	InGameUI->UpdateServerMatchTime(InMatchTimeLeft);
	//}
}

void APlayerHUD::UpdateHealthLeft(const float InHealthRatio)
{
	if (InGameInterface != nullptr)
	{
		InGameInterface->UpdateHealthBar(InHealthRatio);
	}
}

void APlayerHUD::UpdateScoreboardPosition(const int InScoreboardPosition)
{
	if (InGameInterface != nullptr)
	{
		InGameInterface->UpdatePlayerPosition(InScoreboardPosition);
	}
}

void APlayerHUD::ShowMainMenu()
{
	if (MainMenu == nullptr && MainMenuClass != nullptr)
	{
		MainMenu = (UMainMenuScreen*)CreateWidget<UMainMenuScreen>(GetGameInstance(), MainMenuClass);
		MainMenu->AddToViewport(0);
	}

	if (MainMenu != nullptr)
	{
		MainMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::ShowFirstConnectionScreen()
{
	if (FirstConnectionScreen == nullptr && FirstConnectionScreenClass != nullptr)
	{
		FirstConnectionScreen = (UFirstConnectionScreen*)CreateWidget<UFirstConnectionScreen>(GetGameInstance(), FirstConnectionScreenClass);
		FirstConnectionScreen->AddToViewport(1);
	}

	if (FirstConnectionScreen != nullptr)
	{
		FirstConnectionScreen->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::ShowJoinGameScreen()
{
	if (JoinGameScreen == nullptr && JoinGameScreenClass != nullptr)
	{
		JoinGameScreen = (UJoinGameScreen*)CreateWidget<UJoinGameScreen>(GetGameInstance(), JoinGameScreenClass);
		JoinGameScreen->AddToViewport(1);
	}

	if (JoinGameScreen != nullptr)
	{
		JoinGameScreen->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::ShowHostGameScreen()
{
	if (HostGameScreen == nullptr && HostGameScreenClass != nullptr)
	{
		HostGameScreen = (UHostGameScreen*)CreateWidget<UHostGameScreen>(GetGameInstance(), HostGameScreenClass);
		HostGameScreen->AddToViewport(1);
	}

	if (HostGameScreen != nullptr)
	{
		HostGameScreen->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::ShowGameLobbyScreen()
{
	if (GameLobbyScreen == nullptr && GameLobbyScreenClass != nullptr)
	{
		GameLobbyScreen = (UGameLobbyScreen*)CreateWidget<UGameLobbyScreen>(GetGameInstance(), GameLobbyScreenClass);
		GameLobbyScreen->AddToViewport(1);
	}

	if (GameLobbyScreen != nullptr)
	{
		GameLobbyScreen->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::HideGameLobbyScreen()
{
	if (GameLobbyScreen != nullptr)
	{
		GameLobbyScreen->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayerHUD::ShowInGameInterface()
{
	if (InGameInterface == nullptr && InGameInterfaceClass != nullptr)
	{
		InGameInterface = (UInGameInterface*)CreateWidget<UInGameInterface>(GetGameInstance(), InGameInterfaceClass);
		InGameInterface->AddToViewport(1);
	}

	if (InGameInterface != nullptr)
	{
		InGameInterface->SetVisibility(ESlateVisibility::Visible);
	}
}

void APlayerHUD::HideInGameInterface()
{
	if (InGameInterface != nullptr)
	{
		InGameInterface->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayerHUD::ShowScoreBoard()
{
	if (ScoreBoardScreen == nullptr && ScoreBoardScreenClass != nullptr)
	{
		ScoreBoardScreen = (UScoreBoard*)CreateWidget<UScoreBoard>(GetGameInstance(), ScoreBoardScreenClass);
		ScoreBoardScreen->AddToViewport(2);
	}

	if (ScoreBoardScreen != nullptr)
	{
		ScoreBoardScreen->SetVisibility(ESlateVisibility::Visible);
	}
	
}

void APlayerHUD::HideScoreBoard()
{
	if (ScoreBoardScreen != nullptr)
	{
		ScoreBoardScreen->SetVisibility(ESlateVisibility::Hidden);
	}
}