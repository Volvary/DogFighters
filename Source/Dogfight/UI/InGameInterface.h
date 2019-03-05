// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameInterface.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UInGameInterface : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateHealthBar(float NewRatio);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdatePlayerPosition(int NewPosition);
};
