// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "ProjectileBase.h"
#include "TimerManager.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;
}

void AProjectileBase::Server_DisableProjectile_Implementation()
{
	DisableProjectile();
}
bool AProjectileBase::Server_DisableProjectile_Validate()
{
	return true;
}

void AProjectileBase::Server_EnableProjectile_Implementation()
{
	EnableProjectile();
}
bool AProjectileBase::Server_EnableProjectile_Validate()
{
	return true;
}

void AProjectileBase::EnableProjectile_Implementation()
{
	bActive = true;
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}

void AProjectileBase::DisableProjectile_Implementation()
{
	bActive = false;
	SetActorTransform(FTransform(FVector::ZeroVector));
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	//Clear Timer in case the projectile disabled itself due to something else than time (Impact for example)
	GetWorldTimerManager().ClearTimer(LifetimeHandle);
}

void AProjectileBase::SetLifetime(float NewLifetime)
{
	GetWorldTimerManager().SetTimer(LifetimeHandle, this, &AProjectileBase::DisableProjectile, NewLifetime, false);
}

void AProjectileBase::Server_SetStartPoint_Implementation(FTransform NewTransform)
{
	SetActorTransform(NewTransform);

	Replicate_SetStartPoint(NewTransform);
}
bool AProjectileBase::Server_SetStartPoint_Validate(FTransform NewTransform)
{
	return true;
}

void AProjectileBase::Replicate_SetStartPoint_Implementation(FTransform NewTransform)
{
	SetActorTransform(NewTransform);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

