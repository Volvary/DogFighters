// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PlayerInformation/PlayerScore.h"

#include "ScoreBoard.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UScoreBoard : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddPlayer(const FPlayerScore NewPlayer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdatePlayer(const FPlayerScore PlayerToUpdate);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemovePlayer(const FPlayerScore PlayerToRemove);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BulkUpdateBoard(const TArray<FPlayerScore>& Players, bool bClearExisting);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReceiveServerName(const FText& InServerName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReceiveServerTimeLeft(const int InTimeLeft);

};
