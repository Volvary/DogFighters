// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerInformation/PlayerInformation.h"
#include "PlayerInformation/PlayerScore.h"
#include "DogfighterPlayerController.generated.h"

class APlaneCharacter;
class APlayerHUD;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ADogfighterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient)
	APlaneCharacter* PlanePawn;

	UPROPERTY(Transient)
	APlayerHUD* PlayerHUD;

	UPROPERTY(Transient)
	FPlayerInformation PlayerInformation;

	UPROPERTY()
	bool bObtainedPlayerInformation = false;

	UPROPERTY(Replicated)
	bool bCanRequestRespawn = false;

	UPROPERTY()
	int SecondsUntilRespawn = -1;


	UPROPERTY()
	FTimerHandle MinRespawnTimeHandle;
	
	UPROPERTY()
	FTimerHandle MaxRespawnTimeHandle;
	
	UPROPERTY()
	FTimerHandle VisualTimerHandle;

	DECLARE_DELEGATE_OneParam(FCanRespawn, bool);

public:

	UFUNCTION()
	void ControlledCharacterDied(APlaneCharacter* Dead, AActor* Killer);

	UFUNCTION(Client, Reliable)
	void AddNewPlayerToGameLobbyScreen(FPlayerInformation NewPlayer);

	UFUNCTION(Client, Reliable)
	void UpdatePlayerInformation(FPlayerInformation PlayerInfo);

	UFUNCTION(Client, Reliable)
	void UpdateHealtRemaining(float InHealthRatio);

	UFUNCTION(Client, Reliable)
	void ShowGameLobbyScreen();

	UFUNCTION(Client, Reliable)
	void HideGameLobbyScreen();

	UFUNCTION(Client, Reliable)
	void ShowInGameInterface();

	UFUNCTION(Client, Reliable)
	void HideInGameInterface();

	UFUNCTION(Client, Reliable)
	void ShowScoreBoard();

	UFUNCTION(Client, Reliable)
	void HideScoreBoard();

private:
	UFUNCTION()
	void RequestRespawn();

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_RequestRespawn();

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SetRespawnTimer(float MinTime, float MaxTime);

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_SetCanRespawn(bool CanRespawn);

	UFUNCTION(Client, Reliable)
	void Client_SetCanRespawn(bool CanRespawn);

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_RespawnTimerDone();

	UFUNCTION(Client, Reliable)
	void Replicate_SetTimeUntilRespawn(float MinTimeUntilRespawn);

	UFUNCTION(Client, Unreliable)
	void UpdateSecondsUntilRespawn();

public:
	UFUNCTION(Client, Reliable)
	void OnPlayerRespawned();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FPlayerInformation GetPlayerInformation();

	UFUNCTION(Client, Reliable)
	void Client_RequestPlayerInfo();

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_ReceivePlayerInfo(FPlayerInformation PlayerInfo);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ReceiveScoreUpdate(const TArray<FPlayerScore>& InLeaderboard);

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_ReceiveServerName(const FText& ServerName);

	UFUNCTION(Client, Reliable)
	void Client_ReceiveServerName(const FText& ServerName);

	UFUNCTION(Server, WithValidation, Reliable)
	void Server_ReceiveTimeLeftUpdate(const int InMatchTimeRemaining);

	UFUNCTION(Client, Reliable)
	void Client_ReceiveTimeLeftUpdate(const int InMatchTimeRemaining);

	UFUNCTION(Client, Reliable)
	void Client_PopulateScoreBoard(const TArray<FPlayerScore>& LoggedPlayers);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EndOfRound();

	UFUNCTION(Client, Reliable)
	void Client_EndOfRound();

protected:
	virtual void SetupInputComponent() override;

	UFUNCTION(Client, Reliable)
	void Client_ToggleInput(bool Active);

	void CheckHUDReference();
};
