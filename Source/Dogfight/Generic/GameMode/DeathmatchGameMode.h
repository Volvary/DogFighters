// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "PlayGameMode.h"
#include "DeathmatchGameMode.generated.h"

class ADogfighterPlayerController;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API ADeathmatchGameMode : public APlayGameMode
{
	GENERATED_BODY()

protected:

	//Used to set the remaining time.
	int TimeRemaining = 0;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int TimeLimit = 300;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int KillGoal = -1;

	FTimerHandle GameTimerHandle;
	FTimerHandle GameEndHandle;

public:
	void StartMatch() override;

	//Override from PlayGameMode
	int UpdateScore(ADogfighterPlayerController* Player, int Score) override;
	
protected:

	void UpdateTimer();

	void UpdatePlayersOnTimeRemaining();
};
