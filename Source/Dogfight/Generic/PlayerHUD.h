// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerInformation/PlayerInformation.h"
#include "PlayerHUD.generated.h"

class UMainMenuScreen;
class URespawnScreen;
class UFirstConnectionScreen;
class UJoinGameScreen;
class UHostGameScreen;
class UGameLobbyScreen;
class UInGameInterface;
class UScoreBoard;

/**
 * Since HUD is never replicated, all the UI will be spawned here for the Client.
 */
UCLASS()
class DOGFIGHT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
	UMainMenuScreen* MainMenu;

	UPROPERTY(Transient)
	URespawnScreen* RespawnScreen;

	UPROPERTY(Transient)
	UFirstConnectionScreen* FirstConnectionScreen;

	UPROPERTY(Transient)
	UJoinGameScreen* JoinGameScreen;

	UPROPERTY(Transient)
	UHostGameScreen* HostGameScreen;

	UPROPERTY(Transient)
		UGameLobbyScreen* GameLobbyScreen;

	UPROPERTY(Transient)
	UInGameInterface* InGameInterface;

	UPROPERTY(Transient)
	UScoreBoard* ScoreBoardScreen;

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UMainMenuScreen> MainMenuClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<URespawnScreen> RespawnScreenClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UFirstConnectionScreen> FirstConnectionScreenClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UJoinGameScreen> JoinGameScreenClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UHostGameScreen> HostGameScreenClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UGameLobbyScreen> GameLobbyScreenClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UInGameInterface> InGameInterfaceClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UScoreBoard> ScoreBoardScreenClass;


public:

	UFUNCTION(BlueprintCallable)
	URespawnScreen* ValidatedGetRespawnScreen();

	UFUNCTION()
	UScoreBoard* ValidatedGetScoreBoard(bool bShowIfCreated = false);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void AddNewPlayerToGameLobbyScreen(FPlayerInformation NewPlayer);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void RemovePlayerFromGameLobbyScreen(FPlayerInformation RemovedPlayer);

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void UpdatePlayerInformation(FPlayerInformation RemovedPlayer);

	///When in lobby, show the Server Name
	UFUNCTION(BlueprintCallable)
	void ReceiveServerName(const FText& InServerName);

	UFUNCTION()
	void UpdateServerTimeLeft(const int InMatchTimeLeft);

	///Receive Health of plane as Ratio of CurrentHealth/MaxHealth
	UFUNCTION()
	void UpdateHealthLeft(const float InHealthRatio);

	UFUNCTION()
	void UpdateScoreboardPosition(const int InScoreboardPosition);

	//////////////////////////////////////////////////////////////////////////
	//Show/Hide functions for Screens

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();
	
	UFUNCTION(BlueprintCallable)
	void ShowFirstConnectionScreen();

	UFUNCTION(BlueprintCallable)
	void ShowJoinGameScreen();

	UFUNCTION(BlueprintCallable)
	void ShowHostGameScreen();

	UFUNCTION(BlueprintCallable)
	void ShowGameLobbyScreen();

	UFUNCTION(BlueprintCallable)
	void HideGameLobbyScreen();

	UFUNCTION(BlueprintCallable)
	void ShowInGameInterface();

	UFUNCTION(BlueprintCallable)
	void HideInGameInterface();

	UFUNCTION(BlueprintCallable)
	void ShowScoreBoard();

	UFUNCTION(BlueprintCallable)
	void HideScoreBoard();



	void BeginPlay() override;
};