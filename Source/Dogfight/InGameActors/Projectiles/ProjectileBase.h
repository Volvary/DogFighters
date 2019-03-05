// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProjectileBase.generated.h"

UCLASS()
class DOGFIGHT_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	
	//Used for Pooling
	UPROPERTY(BlueprintReadOnly)
	bool bActive;

	FTimerHandle LifetimeHandle;

public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	//Called by the PoolingManager.
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_EnableProjectile();

	//Called by the PoolingManager.
	UFUNCTION(Server, WithValidation, Reliable)
	void Server_DisableProjectile();

	UFUNCTION(NetMulticast, Reliable)
	void EnableProjectile();

	UFUNCTION(NetMulticast, Reliable)
	void DisableProjectile();

	void SetLifetime(float NewLifetime);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetStartPoint(FTransform NewTransform);

	UFUNCTION(NetMulticast, Reliable)
	void Replicate_SetStartPoint(FTransform NewTransform);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
