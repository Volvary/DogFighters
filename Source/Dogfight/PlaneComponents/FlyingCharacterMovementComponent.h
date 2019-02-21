// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FlyingCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API UFlyingCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual void PhysFlying(float deltaTime, int32 Iterations) override;
};
