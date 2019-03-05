#pragma once

#include "PlayerScore.generated.h"

USTRUCT(BlueprintType)
struct FPlayerScore
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int Score;
};