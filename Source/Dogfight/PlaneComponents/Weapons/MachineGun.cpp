// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "MachineGun.h"
#include "CollisionQueryParams.h"
#include "InGameActors/PlaneCharacter.h"

#include "Generic/PoolingManager.h"

AMachineGun::AMachineGun()
{
	Damage = 5;
	FireRate = 0.2f;
	AutomaticFire = true;

	ProjectileSpawnOffset = FVector(50.0f, 50.0f, 0.0f);
}

#pragma optimize("",off)
void AMachineGun::Fire()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;

	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(WeaponOwner);

	FTransform Origin = GetProjectileOrigin();

	FVector OriginPoint = Origin.GetLocation();

	FVector EndPoint = OriginPoint + (Origin.GetRotation().GetForwardVector() * MaxHitScanRange);

	FireHitscan(&HitResult,&Params);

	if (HitResult.Actor != nullptr)
	{
		FDamageEvent Event;

		HitResult.Actor->TakeDamage(Damage, Event, WeaponOwner->GetController(), this);
	}

	if (PoolingManager != nullptr && ProjectileClass != nullptr)
	{
		AProjectileBase* Beam = PoolingManager->GetProjectileOfClass(ProjectileClass);

		if (Beam != nullptr)
		{
			FTransform StartPoint;
			
			FVector BeamScale = Beam->GetActorScale3D();

			StartPoint.SetLocation(OriginPoint + (Origin.GetRotation().GetForwardVector() * (MaxHitScanRange*0.5)));
			StartPoint.SetRotation(Origin.GetRotation());
			StartPoint.SetScale3D(FVector(BeamScale.X,MaxHitScanRange*0.05,BeamScale.Z));

			Beam->Server_SetStartPoint(StartPoint);
			
			Beam->SetLifetime(ProjectileLifeTime);
		}
		
	}
	

	SwitchOffset();
}
#pragma optimize("",on)

void AMachineGun::SameWeaponFired()
{
	SwitchOffset();
}

void AMachineGun::SwitchOffset()
{
	ProjectileSpawnOffset = ProjectileSpawnOffset * CannonAlternator;
}
