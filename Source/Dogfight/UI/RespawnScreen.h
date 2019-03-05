// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RespawnScreen.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API URespawnScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	int RemainingTime;

	UPROPERTY(BlueprintReadOnly)
	FString DeathText;

	UPROPERTY(BlueprintReadOnly)
	bool bCanRespawn;


public:

	UFUNCTION()
	void SetRemainingTime(const int InTime) { RemainingTime = InTime; }

	UFUNCTION()
	void SetDeathText(const FString& InText) { DeathText = InText; }

	UFUNCTION()
	void SetCanRespawn(const bool InCanRespawn) { bCanRespawn = InCanRespawn; }

};
