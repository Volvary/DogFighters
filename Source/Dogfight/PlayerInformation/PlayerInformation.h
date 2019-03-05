// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "Engine/Texture2D.h"

#include "PlayerInformation.generated.h"

class AWeaponBase;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerInformation
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = Basic)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, Category = Basic)
	UTexture2D* PlayerIcon;

	UPROPERTY(BlueprintReadWrite, Category = Basic)
	TSubclassOf<AWeaponBase> PrimaryWeaponClass;

	UPROPERTY(BlueprintReadWrite, Category = Basic)
	TSubclassOf<AWeaponBase> SecondaryWeaponClass;

	UPROPERTY(BlueprintReadWrite, Category = Basic)
	UTexture2D* PlaneTexture;
};
