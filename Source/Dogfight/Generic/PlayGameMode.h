// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayGameMode.generated.h"

class UPoolingManager;

/**
 * The Base for any of the specific game modes. Defines the main logic, specific logic (teams, etc.) in childs.
 */
UCLASS()
class DOGFIGHT_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY()
	UPoolingManager* PoolingManager = nullptr;

public:

	APlayGameMode();

	void BeginPlay() override;

	virtual UPoolingManager* GetPoolingManager();

};
