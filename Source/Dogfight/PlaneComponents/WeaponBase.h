// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InGameActors/Projectiles/ProjectileBase.h"

#include "WeaponBase.generated.h"

class APlaneCharacter;
class APoolingManager;

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
protected:
	/** Whether or not this weapon fires automatic.*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool AutomaticFire = true;

	/** The speed this weapon fires at. (In seconds, as delay between each shot)*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float FireRate;

	/** The speed this weapon fires at. (In seconds, as delay between each shot)*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float Damage = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float ProjectileLifeTime;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FVector ProjectileSpawnOffset;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MaxHitScanRange = 500;

	APlaneCharacter* WeaponOwner;

	APoolingManager* PoolingManager;

public:

	AWeaponBase();
	
	virtual void Fire() {}

	virtual void SameWeaponFired() {}

	UFUNCTION(BlueprintCallable)
	virtual bool CanFireAutomatic() {return AutomaticFire;}

	UFUNCTION(BlueprintCallable)
	virtual float GetFireRate() {return FireRate;}

	UFUNCTION(BlueprintCallable)
	virtual float GetDamage() {return Damage;}

	UFUNCTION(BlueprintCallable)
	virtual float GetProjectileLifetime() {return ProjectileLifeTime;}

	UFUNCTION(BlueprintCallable)
	virtual FTransform GetProjectileOrigin();

	UFUNCTION(BlueprintCallable)
	virtual TSubclassOf<AProjectileBase> GetProjectileClass();

	void SetWeaponOwner(APlaneCharacter* NewOwner);

	void SetPoolingManager(APoolingManager* Pooling);
	
protected:

	void FireHitscan(FVector Origin, FVector End, FHitResult* Hit, FCollisionQueryParams* TraceParams);
	void FireHitscan(FHitResult* Hit, FCollisionQueryParams* TraceParams);
	
};
