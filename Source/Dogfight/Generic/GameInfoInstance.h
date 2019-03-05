// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerInformation/PlayerInformation.h"
#include "GameInfoInstance.generated.h"


class APlayerHUD;
class UDogfightSaveGame;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UGameInfoInstance : public UGameInstance
{
	GENERATED_BODY()


	UPROPERTY(Transient)
	UDogfightSaveGame* PlayerSettingsSave;

	UPROPERTY(Transient)
	FPlayerInformation PlayerInformation;

	UPROPERTY(Transient)
	APlayerHUD* PlayerHUD;

public:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Server Settings")
	int MaxPlayers;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Server Settings")
	FText ServerName;

public:

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void SavePlayerData(FPlayerInformation InPlayerInfo);

	UFUNCTION(BlueprintCallable)
	FPlayerInformation GetPlayerData();

protected:

	UFUNCTION()
	void CheckSavedData();

	UFUNCTION()
	void LoadSavedData(bool bQuelchMessage = false);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LaunchLobby(int InNumOfPlayers, bool InEnableLan, const FText& InServerName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DestroySession();

	UFUNCTION(BlueprintCallable)
	void OnSessionDestroyed();

};
