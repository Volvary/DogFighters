// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "PlaneComponents/WeaponBase.h"
#include "MachineGun.generated.h"

/**
 * 
 */
UCLASS()
class DOGFIGHT_API AMachineGun : public AWeaponBase
{
	GENERATED_BODY()

	//Used to shift which side of the plane fires.
	FVector CannonAlternator = FVector(1,-1, 1);

public:

	AMachineGun();

	void Fire() override;

	void SameWeaponFired() override;

protected:

	void SwitchOffset();
};
