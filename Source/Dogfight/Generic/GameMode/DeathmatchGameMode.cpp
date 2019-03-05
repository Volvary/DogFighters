// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "DeathmatchGameMode.h"
#include "Generic/GameMode/PlayGameMode.h"
#include "TimerManager.h"
#include "Generic/DogfighterPlayerController.h"

void ADeathmatchGameMode::StartMatch()
{
	Super::StartMatch();

	TimeRemaining = TimeLimit;
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ADeathmatchGameMode::UpdateTimer, 1.0f, true);
}

int ADeathmatchGameMode::UpdateScore(ADogfighterPlayerController* Player, int Score)
{
	int FinalScore = Super::UpdateScore(Player,Score);

	if (FinalScore >= KillGoal)
	{
		//Prepare for ending the round.

		//Give 0.5 second to the server to process any potential collision/explosion killing multiple players.
		GetWorldTimerManager().SetTimer(GameEndHandle, this, &ADeathmatchGameMode::EndMatch, 0.5f, false);
		GetWorldTimerManager().ClearTimer(GameTimerHandle);
	}

	return FinalScore;
}

void ADeathmatchGameMode::UpdateTimer()
{
	TimeRemaining -= 1;

	if (TimeRemaining % 10)
	{
		UpdatePlayersOnTimeRemaining();
	}

	if (TimeRemaining <= 0)
	{
		EndMatch();
		GetWorldTimerManager().ClearTimer(GameTimerHandle);
	}
}

void ADeathmatchGameMode::UpdatePlayersOnTimeRemaining()
{
	//TODO: Get All Players
	// Pass the current time to players.

	//for()
}
