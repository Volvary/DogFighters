// Portfolio Project by Xavier Auger-Mailhot (February 2019's Game-A-Month)

#include "PlaneCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

#include "PlaneComponents/FlyingCharacterMovementComponent.h"
#include "PlaneComponents/WeaponBase.h"

#include "Generic/GameMode/PlayGameMode.h"
#include "Generic/PlayerHUD.h"
#include "Generic/DogfighterPlayerController.h"

#include "Dogfight.h"

#include "DamageTypes/DamageImpact.h"

APlaneCharacter::APlaneCharacter(const FObjectInitializer& OI)
	: Super(OI.SetDefaultSubobjectClass<UFlyingCharacterMovementComponent>(APlaneCharacter::CharacterMovementComponentName))
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create static mesh component
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	PlaneMesh->SetupAttachment(GetCapsuleComponent());

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(PlaneMesh);	// Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 160.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f,0.f,60.f);
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 15.f;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	// Set handling parameters
	Acceleration = 500.f;
	TurnSpeed = 50.f;
	MaxSpeed = 4000.f;
	MinSpeed = 500.f;
	CurrentForwardSpeed = 500.f;

	//Set Movement parameters
	bUseControllerRotationYaw = false;
	
	MovementComp = (UCharacterMovementComponent*) GetMovementComponent();

	MovementComp->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	MovementComp->DefaultWaterMovementMode = EMovementMode::MOVE_Flying;
	MovementComp->UpdatedComponent = RootComponent;

	//Weapon Confirmations
	if (PrimaryWeaponClass == AWeaponBase::StaticClass())
	{
		UE_LOG(LogFlying, Error, TEXT("Cannot instanciate a UWeaponBase. This class is meant to be overloaded. (Primary weapon class)"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Cannot instanciate a UWeaponBase. Please change primary weapon class.");
	}
	if (SecondaryWeaponClass == AWeaponBase::StaticClass())
	{
		UE_LOG(LogFlying, Error, TEXT("Cannot instanciate a UWeaponBase. This class is meant to be overloaded. (Secondary weapon class)"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Cannot instanciate a UWeaponBase. Please change secondary weapon class.");
	}
}

float APlaneCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.DamageTypeClass == UDamageImpact::StaticClass())
	{
		ActualDamage *= ImpactResilience;
	}

	if (ActualDamage > 0.f)
	{
		CurrentHealth -= ActualDamage;

		ADogfighterPlayerController* PC = Cast<ADogfighterPlayerController>(GetController());

		if (PC != nullptr)
		{
			PC->UpdateHealtRemaining(CurrentHealth/MaxHealth);
		}

		if (CurrentHealth <= 0)
		{
			Server_OnPlayerDeath(DamageCauser);
		}
	}

	return ActualDamage;
	
}

// Called when the game starts or when spawned
void APlaneCharacter::BeginPlay()
{
	Super::BeginPlay();

	APoolingManager* PoolingManager = nullptr;

	//PoolingManager is held serverside (in GM), so no use in pulling it ClientSide.
	if(HasAuthority()){
		PoolingManager = Server_GetPoolingManager();
	}

	//Weapon Init
	if (PrimaryWeaponClass == AWeaponBase::StaticClass())
	{
		UE_LOG(LogFlying, Error, TEXT("Cannot instanciate a UWeaponBase. This class is meant to be overloaded. (Primary weapon class)"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Cannot instanciate a UWeaponBase. Please change primary weapon class.");
	}
	else if (PrimaryWeaponClass != nullptr)
	{
		PrimaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(PrimaryWeaponClass);
		PrimaryWeapon->SetWeaponOwner(this);
		PrimaryWeapon->SetPoolingManager(PoolingManager);
		PrimaryWeapon->AttachToActor(this,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative, true));
	}
			
	if (SecondaryWeaponClass == AWeaponBase::StaticClass())
	{
		UE_LOG(LogFlying, Error, TEXT("Cannot instanciate a UWeaponBase. This class is meant to be overloaded. (Secondary weapon class)"));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Cannot instanciate a UWeaponBase. Please change secondary weapon class.");
	}
	else if (SecondaryWeaponClass != nullptr)
	{
		SecondaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(SecondaryWeaponClass);
		SecondaryWeapon->SetWeaponOwner(this);
		SecondaryWeapon->SetPoolingManager(PoolingManager);
		SecondaryWeapon->AttachToActor(this,FAttachmentTransformRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepRelative, true));
	}
	
	bPrimaryWeaponReady = bSecondaryWeaponReady = true;
}

void APlaneCharacter::Server_PrimaryWeaponFire_Implementation()
{
	if (PrimaryWeapon != nullptr && bAlive)
	{
		PrimaryWeapon->Fire();

		if (PrimaryWeapon->StaticClass == SecondaryWeapon->StaticClass && SecondaryWeapon != nullptr)
		{
			SecondaryWeapon->SameWeaponFired();
		}

		GetWorldTimerManager().SetTimer(PrimaryFireTimer, this, &APlaneCharacter::Server_PrimaryWeaponReady, PrimaryWeapon->GetFireRate(), false);
	}
}
bool APlaneCharacter::Server_PrimaryWeaponFire_Validate()
{
	return true;
}


void APlaneCharacter::Server_SecondaryWeaponFire_Implementation()
{
	if (SecondaryWeapon != nullptr && bAlive)
	{
		SecondaryWeapon->Fire();

		if (PrimaryWeapon->StaticClass == SecondaryWeapon->StaticClass && PrimaryWeapon != nullptr)
		{
			PrimaryWeapon->SameWeaponFired();
		}

		GetWorldTimerManager().SetTimer(SecondaryFireTimer, this, &APlaneCharacter::Server_SecondaryWeaponReady, SecondaryWeapon->GetFireRate(), false);
	}
}
bool APlaneCharacter::Server_SecondaryWeaponFire_Validate()
{
	return true;
}

void APlaneCharacter::Server_PrimaryWeaponReady_Implementation()
{
	if (PrimaryWeapon != nullptr)
	{
		if (bFiringPrimaryWeapon && PrimaryWeapon->CanFireAutomatic() && bAlive)
		{
			Server_PrimaryWeaponFire();
		}
		else
		{
			bPrimaryWeaponReady = true;
		}
	}
}
bool APlaneCharacter::Server_PrimaryWeaponReady_Validate()
{
	return true;
}


void APlaneCharacter::Server_SecondaryWeaponReady_Implementation()
{
	if (SecondaryWeapon != nullptr)
	{
		if (bFiringSecondaryWeapon && SecondaryWeapon->CanFireAutomatic() && bAlive)
		{
			Server_SecondaryWeaponFire();
		}
		else
		{
			bSecondaryWeaponReady = true;
		}
	}
}
bool APlaneCharacter::Server_SecondaryWeaponReady_Validate()
{
	return true;
}

void APlaneCharacter::Server_OnPlayerDeath_Implementation(AActor* Killer)
{	
	Replicate_OnPlayerDeath();

	if (Controller != nullptr)
	{
		ADogfighterPlayerController* PlayerController = Cast<ADogfighterPlayerController>(Controller);

		if (PlayerController != nullptr)
		{
			PlayerController->ControlledCharacterDied(this, Killer);
		}
	}

	//TODO:
	//Show Death log (X killed you with Y)
	//Switch to Death Cam

}
bool APlaneCharacter::Server_OnPlayerDeath_Validate(AActor* Killer){ return true; }

void APlaneCharacter::Replicate_OnPlayerDeath_Implementation()
{
	Replicate_DisablePlane();

	if(PlayerController != nullptr){
		PlayerController->UpdateHealtRemaining(0.0f);
	}

	//TODO: Play Clienside Explosion
}

void APlaneCharacter::Server_OnPlayerRespawn_Implementation(FTransform RespawnLocation)
{
	SetActorTransform(RespawnLocation);

	CurrentForwardSpeed = MinSpeed;

	CurrentHealth = MaxHealth;

	//Show the Plane
	PlaneMesh->SetHiddenInGame(false);
	SetActorEnableCollision(true);

	Replicate_OnPlayerRespawn(RespawnLocation);

	bAlive = true;
}
bool APlaneCharacter::Server_OnPlayerRespawn_Validate(FTransform RespawnLocation)
{
	return true;
}

void APlaneCharacter::Replicate_OnPlayerRespawn_Implementation(FTransform RespawnLocation)
{
	SetActorTransform(RespawnLocation);

	CurrentHealth = MaxHealth;

	Client_OnPlayerRespawn();

	//Show the Plane
	PlaneMesh->SetHiddenInGame(false);
	SetActorEnableCollision(true);
}

// Called every frame
void APlaneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlaneCharacter::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	if (HasAuthority())
	{
		float AngleOfImpact = FMath::RadiansToDegrees(acosf(FVector::DotProduct(GetActorForwardVector(), HitNormal)));

		//If the player hits a wall too directly, kill the player.
		if (AngleOfImpact > 170)
		{
			Server_OnPlayerDeath(Other);
		}

		FDamageEvent Event;
		Event.DamageTypeClass = UDamageImpact::StaticClass();

		Other->TakeDamage((AngleOfImpact - 90), Event, GetController(), Other);

		TakeDamage((AngleOfImpact - 90), Event, GetController(), Other);

	}

	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

void APlaneCharacter::FlyingTick(float DeltaTime, bool LastIteration)
{
	FRotator NewRotation = FRotator(DeltaTime * CurrentPitchSpeed, DeltaTime * CurrentYawSpeed, DeltaTime * CurrentRollSpeed);

	MovementComp->MoveUpdatedComponent(FVector::ZeroVector, NewRotation + GetActorRotation(), true);
	MovementComp->Velocity = GetActorForwardVector() * CurrentForwardSpeed;

}

void APlaneCharacter::ThrustInput(float Val)
{
	if (IsLocallyControlled() == true)
	{
		Server_ThrustInput(Val);
	}
}

void APlaneCharacter::Server_ThrustInput_Implementation(float Val)
{
	if (!bAlive)
	{
		return;
	}

	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * Acceleration) : (-0.5f * Acceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}
bool APlaneCharacter::Server_ThrustInput_Validate(float Val)
{
	return true;
}

void APlaneCharacter::MoveUpInput(float Val)
{
	if (IsLocallyControlled() == true)
	{
		Server_MoveUpInput(Val);
	}
}

void APlaneCharacter::Server_MoveUpInput_Implementation(float Val)
{
	if (!bAlive)
	{
		return;
	}

	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}
bool APlaneCharacter::Server_MoveUpInput_Validate(float Val)
{
	return true;
}

void APlaneCharacter::MoveRightInput(float Val)
{
	if (IsLocallyControlled() == true)
	{
		Server_MoveRightInput(Val);
	}
}

void APlaneCharacter::Server_MoveRightInput_Implementation(float Val)
{
	if (!bAlive)
	{
		return;
	}

	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value.
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}
bool APlaneCharacter::Server_MoveRightInput_Validate(float Val)
{
	return true;
}

void APlaneCharacter::FirePrimaryWeaponInput(bool Pressed)
{
	if (IsLocallyControlled() == true)
	{
		Server_FirePrimaryWeaponInput(Pressed);
	}
}

void APlaneCharacter::Server_FirePrimaryWeaponInput_Implementation(bool Pressed)
{
	if (PrimaryWeapon == nullptr)
	{
		return;
	}

	bFiringPrimaryWeapon = Pressed;
	
	if (bFiringPrimaryWeapon && CanFireWeapon(PrimaryWeapon))
	{
		Server_PrimaryWeaponFire();
	}
}
bool APlaneCharacter::Server_FirePrimaryWeaponInput_Validate(bool Pressed)
{
	return true;
}

void APlaneCharacter::FireSecondaryWeaponInput(bool Pressed)
{
	if (IsLocallyControlled() == true)
	{
		Server_FireSecondaryWeaponInput(Pressed);
	}
}

void APlaneCharacter::ShowScoreBoard()
{
	CheckPlayerRef();

	if (PlayerController != nullptr)
	{
		PlayerController->ShowScoreBoard();
	}
}

void APlaneCharacter::HideScoreBoard()
{
	CheckPlayerRef();

	if (PlayerController != nullptr)
	{
		PlayerController->HideScoreBoard();
	}
}

APoolingManager * APlaneCharacter::Server_GetPoolingManager()
{
	APoolingManager* PoolingManager = nullptr;

	if (HasAuthority())
	{
		APlayGameMode* GM = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());

		if (GM != nullptr)
		{
			PoolingManager = GM->GetPoolingManager();
		}
	}

	return PoolingManager;
}

void APlaneCharacter::Server_FireSecondaryWeaponInput_Implementation(bool Pressed)
{
	if (SecondaryWeapon == nullptr)
	{
		return;
	}

	bFiringSecondaryWeapon = Pressed;
	
	if (bFiringSecondaryWeapon && CanFireWeapon(SecondaryWeapon))
	{
		Server_SecondaryWeaponFire();
	}
}
bool APlaneCharacter::Server_FireSecondaryWeaponInput_Validate(bool Pressed)
{
	return true;
}

bool APlaneCharacter::CanFireWeapon(AWeaponBase * Weapon)
{
	return Weapon == PrimaryWeapon ? bPrimaryWeaponReady : bSecondaryWeaponReady;
}

void APlaneCharacter::Client_OnPlayerDeath_Implementation()
{

}

void APlaneCharacter::Client_OnPlayerRespawn_Implementation()
{

}

void APlaneCharacter::Replicate_DisablePlane_Implementation()
{
	//Stop the pawn/camera from muving anywhere.
	MovementComp->StopMovementImmediately();
	CurrentForwardSpeed = 0;

	//Hide the Plane
	PlaneMesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);

	Client_OnPlayerDeath();

	//Disable any Inputs
	bAlive = false;
}

// Called to bind functionality to input
void APlaneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Check if PlayerInputComponent is valid (not NULL)
	check(PlayerInputComponent);

	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("Thrust", this, &APlaneCharacter::ThrustInput);
	PlayerInputComponent->BindAxis("MoveUp", this, &APlaneCharacter::MoveUpInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlaneCharacter::MoveRightInput);

	PlayerInputComponent->BindAction<FInputPressed>("FirePrimary", IE_Pressed, this, &APlaneCharacter::FirePrimaryWeaponInput, true);
	PlayerInputComponent->BindAction<FInputPressed>("FirePrimary", IE_Released, this, &APlaneCharacter::FirePrimaryWeaponInput, false);

	PlayerInputComponent->BindAction<FInputPressed>("FireSecondary", IE_Pressed, this, &APlaneCharacter::FireSecondaryWeaponInput, true);
	PlayerInputComponent->BindAction<FInputPressed>("FireSecondary", IE_Released, this, &APlaneCharacter::FireSecondaryWeaponInput, false);


	PlayerInputComponent->BindAction("OpenScoreboard", IE_Pressed, this, &APlaneCharacter::ShowScoreBoard);
	PlayerInputComponent->BindAction("OpenScoreboard", IE_Released, this, &APlaneCharacter::HideScoreBoard);
	
}

void APlaneCharacter::CheckPlayerRef()
{
	if (PlayerController == nullptr)
	{
		PlayerController = Cast<ADogfighterPlayerController>(GetController());
	}
}

