// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/World.h"

#include "PoolingManager.generated.h"

class AProjectileBase;
class UWorld;

/**
 * ServerSide only. Spawned by the GameMode and used by the Server to maintain resources.
 */
UCLASS()
class DOGFIGHT_API APoolingManager : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<AProjectileBase*> ProjectilePool;

public:

	APoolingManager();

	/** Tries to reactivate a disabled projectile of the requested class. Will create one if none are available. Projectile is not moved.*/
	AProjectileBase* GetProjectileOfClass(TSubclassOf<AProjectileBase> ProjectileClass);

	void OnGameEnd();
};
