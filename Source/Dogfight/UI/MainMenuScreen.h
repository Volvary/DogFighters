// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuScreen.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UMainMenuScreen : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OpenFirstConnectionScreen();
	
};
