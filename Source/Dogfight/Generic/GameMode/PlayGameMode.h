// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayerInformation/PlayerInformation.h"
#include "PlayGameMode.generated.h"

class APoolingManager;
class ADogfighterPlayerController;

/**
 * The Base for any of the specific game modes. Defines the main logic, specific logic (teams, etc.) in childs.
 */
UCLASS()
class DOGFIGHT_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	/** The maximum time a player can wait before respawning. Usually much higher than MinTime*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Game Mode")
	float MaxRespawnTime = -2.0f;

	UPROPERTY(BlueprintReadOnly, Category="Game Mode")
	int MaxPlayers = 2;

	UPROPERTY()
	FText ServerName;

protected:

	UPROPERTY()
	APoolingManager* PoolingManager = nullptr;

	UPROPERTY()
	TMap<FString,int> Scores;

	UPROPERTY()
	TArray<FString> Leaderboard;

	UPROPERTY()
	TArray<FString> LastScorers;

	UPROPERTY()
	TMap<ADogfighterPlayerController*, FPlayerInformation> PlayersInServer;

	FTimerHandle EndOfMatchTimer;

	float EndOfMatchWaitDelay = 5.0f;

	ADogfighterPlayerController* ServerHost;

public:

	APlayGameMode();

	void BeginPlay() override;

	virtual APoolingManager* GetPoolingManager();

	UFUNCTION(Server, Reliable, WithValidation)
	void OnPlayerDied(AController* Dead, AActor* Killer);

	///Update player score. Returns that player's new score.
	virtual int UpdateScore(ADogfighterPlayerController* Player, int Score);

	void ReorderLeaderboard();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RequestRespawn(AController* Player);

	void GetRespawnTimer(AController* Dead, float& OutMinTime, float& OutMaxTime);

	void PostLogin(APlayerController* NewPlayer) override;

	void StartMatch() override;

	void EndMatch() override;

	void RequestMatchStart(ADogfighterPlayerController* PlayerRequesting);

	void EndOfMatchTimerFinished();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_UpdatePlayerInformation(AController* Player, FPlayerInformation PlayerInfo);
};
