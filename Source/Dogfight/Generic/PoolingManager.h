// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Engine/World.h"

#include "PoolingManager.generated.h"

class AProjectileBase;
class UWorld;

/**
 * ServerSide only. Spawned by the GameMode and used by the Server to maintain resources.
 */
UCLASS()
class DOGFIGHT_API UPoolingManager : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<AProjectileBase*> ProjectilePool;

	UPROPERTY()
	UWorld* GameModeWorld;

public:

	UPoolingManager();

	/** Tries to reactivate a disabled projectile of the requested class. Will create one if none are available. Projectile is not moved.*/
	AProjectileBase* GetProjectileOfClass(TSubclassOf<AProjectileBase> ProjectileClass);

	void SetWorld(UWorld* GameWorld);

	void OnGameEnd();
};
