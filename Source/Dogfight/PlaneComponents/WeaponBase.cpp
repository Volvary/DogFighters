// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "WeaponBase.h"
#include "InGameActors/Projectiles/ProjectileBase.h"

#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"

#include "InGameActors/PlaneCharacter.h"

#include "Generic/PoolingManager.h"

AWeaponBase::AWeaponBase()
{

}

#pragma optimize("",off)
FTransform AWeaponBase::GetProjectileOrigin()
{
	SetActorRelativeLocation(ProjectileSpawnOffset);

	FTransform Return = GetActorTransform();

	return Return;
}
#pragma optimize("",on)

TSubclassOf<AProjectileBase> AWeaponBase::GetProjectileClass()
{
	return ProjectileClass;
}

void AWeaponBase::SetWeaponOwner(APlaneCharacter * NewOwner)
{
	WeaponOwner = NewOwner;
}

void AWeaponBase::SetPoolingManager(APoolingManager * Pooling)
{
	PoolingManager = Pooling;
}

void AWeaponBase::FireHitscan(FVector Origin, FVector End, FHitResult* Hit, FCollisionQueryParams* TraceParams)
{
	TraceParams->bTraceComplex = true;
    TraceParams->bTraceAsyncScene = true;

	bool DidTrace = GetWorld()->LineTraceSingleByChannel(
         *Hit,        //result
         Origin,        //start
         End,        //end
         ECC_Pawn,    //collision channel
         *TraceParams
         );

}

void AWeaponBase::FireHitscan(FHitResult* Hit, FCollisionQueryParams* TraceParams)
{
	FTransform Origin = GetProjectileOrigin();

	FVector OriginPoint = Origin.GetLocation();

	FVector EndPoint = OriginPoint + (Origin.GetRotation().GetForwardVector() * MaxHitScanRange);

	TraceParams->bTraceComplex = true;
    TraceParams->bTraceAsyncScene = true;

	bool DidTrace = GetWorld()->LineTraceSingleByChannel(
         *Hit,        //result
         OriginPoint,        //start
         EndPoint,        //end
         ECC_Pawn,    //collision channel
         *TraceParams
         );

	//DrawDebugLine(GetWorld(), OriginPoint, EndPoint, FColor::Black);

}