// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInformation/PlayerInformation.h"
#include "GameLobbyScreen.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UGameLobbyScreen : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(Client, Reliable)
	void Replicate_NewPlayerJoined(FPlayerInformation NewPlayer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NewPlayerJoined(FPlayerInformation NewPlayer);

	UFUNCTION(Client, Reliable)
	void Replicate_UpdatePlayerInformation(FPlayerInformation NewPlayer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdatePlayerInformation(FPlayerInformation PlayerUpdated);

	UFUNCTION(Client, Reliable)
	void Replicate_RemovePlayerFromList(FPlayerInformation RemovedPlayer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemovePlayerFromList(FPlayerInformation RemovedPlayer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReceiveServerName(const FText& InServerName);
	
};
