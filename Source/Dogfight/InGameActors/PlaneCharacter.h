// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlaneCharacter.generated.h"

class UCharacterMovementComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

class AWeaponBase;
class UPoolingManager;

UCLASS()
class DOGFIGHT_API APlaneCharacter : public ACharacter
{
	GENERATED_BODY()

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlaneMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	
	UPROPERTY(Transient, EditDefaultsOnly)
	UCharacterMovementComponent* MovementComp;

	/** Currently Loaded Weapons*/
	UPROPERTY()
	AWeaponBase* PrimaryWeapon;
	UPROPERTY()
	AWeaponBase* SecondaryWeapon;
protected:

	/** How quickly forward speed changes */
	UPROPERTY(Category=Plane, EditAnywhere)
	float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category=Plane, EditAnywhere)
	float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Pitch, EditAnywhere)
	float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category=Yaw, EditAnywhere)
	float MinSpeed;

	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	/** Used for Rate Of Fire control.*/
	FTimerHandle PrimaryFireTimer;
	FTimerHandle SecondaryFireTimer;

	bool bPrimaryWeaponReady;
	bool bSecondaryWeaponReady;


	bool bFiringPrimaryWeapon;
	bool bFiringSecondaryWeapon;

	/** Containers for BP inheritance of Weapons.*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AWeaponBase> PrimaryWeaponClass = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AWeaponBase> SecondaryWeaponClass = nullptr;

	DECLARE_DELEGATE_OneParam(FInputPressed, bool);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth = MaxHealth;

public:
	// Constructor required to assign our own CharacterMovementComponent
	APlaneCharacter(const FObjectInitializer& OI);
	
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Begin APawn overrides
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override; // Allows binding actions/axes to functions
	// End APawn overrides


	///Inputs
	/** Bound to the thrust axis */
	void ThrustInput(float Val);
	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_ThrustInput(float Val);
	
	/** Bound to the vertical axis */
	void MoveUpInput(float Val);
	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_MoveUpInput(float Val);

	/** Bound to the horizontal axis */
	void MoveRightInput(float Val);
	UFUNCTION(Server, Unreliable, WithValidation)
	void Server_MoveRightInput(float Val);

	void FirePrimaryWeaponInput(bool Pressed);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_FirePrimaryWeaponInput(bool Pressed);

	void FireSecondaryWeaponInput(bool Pressed);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_FireSecondaryWeaponInput(bool Pressed);
	///End of Inputs

	///Server Setup
	/** Only available on server. Will give nullptr on clients.*/
	UPoolingManager* Server_GetPoolingManager();
	
	///Server Weapon Firing
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PrimaryWeaponFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SecondaryWeaponFire();

	bool CanFireWeapon(AWeaponBase* Weapon);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PrimaryWeaponReady();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SecondaryWeaponReady();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	// End AActor overrides

	/**
	* --------------------------------------------
	* Entry point for handling of flying movement.
	* --------------------------------------------
	* Added a "FlyingTick" into CharacterMovementComponent because calculating the movement
	* from within the physics loop results in high accuracy that wont cause significant 
	* indiscrepancies at varied frame rates.
	*/
	UFUNCTION(Category = "Character Movement: Flying")
	void FlyingTick(float DeltaTime, bool LastIteration);
};
