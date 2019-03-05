// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerInformation/PlayerInformation.h"

#include "DogfightSaveGame.generated.h"


/**
 * 
 */

UCLASS()
class DOGFIGHT_API UDogfightSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, Category = Basic, Replicated)
	FPlayerInformation PlayerInfo;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName = "PlayerSettingsSave";

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;

	UDogfightSaveGame();
};
