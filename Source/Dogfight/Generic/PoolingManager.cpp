// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "PoolingManager.h"

#include "Engine/World.h"

#include "Dogfight.h"
#include "InGameActors/Projectiles/ProjectileBase.h"

UPoolingManager::UPoolingManager()
{
	
}

AProjectileBase * UPoolingManager::GetProjectileOfClass(TSubclassOf<AProjectileBase> ProjectileClass)
{
	if (ProjectileClass == nullptr || ProjectileClass == AProjectileBase::StaticClass())
	{
		UE_LOG(LogFlying, Error, TEXT("An invalid type was passed to GetProjectileOfClass(). Please make sure it is not null or AProjectileBase."));
	
		return nullptr;
	}

	AProjectileBase* Return = nullptr;

	if (ProjectilePool.Num() != 0)
	{
		for (AProjectileBase* Projectile : ProjectilePool)
		{
			if (Projectile != nullptr && Projectile->StaticClass() == ProjectileClass)
			{
				if (!Projectile->bActive)
				{
					Projectile->EnableProjectile();
					break;
				}
			}
		}
	}
	
	//If no valid Projectile was found, spawn a new one.
	if (Return == nullptr)
	{
		FActorSpawnParameters ProjectileSpawnParams = FActorSpawnParameters();
		ProjectileSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Return = GameModeWorld->SpawnActor<AProjectileBase>(ProjectileClass, ProjectileSpawnParams);

		if (Return != nullptr)
		{
			ProjectilePool.Add(Return);
		}
		else
		{
			UE_LOG(LogFlying, Error, TEXT("Something strange happened. A Projectile could not be spawned for some reason."));
		}
	}
	
	//Enable the projectile for everyone before it's used.
	Return->EnableProjectile();
	return Return;
}

void UPoolingManager::SetWorld(UWorld * GameWorld)
{
	GameModeWorld = GameWorld;
}

void UPoolingManager::OnGameEnd()
{
	//Destroy all spawned Projectiles since we cannot be sure we will need them next round.
	for (AProjectileBase* Projectile : ProjectilePool)
	{
		if (Projectile != nullptr)
		{
			Projectile->Destroy();
		}
	}

	ProjectilePool.Empty();
}
