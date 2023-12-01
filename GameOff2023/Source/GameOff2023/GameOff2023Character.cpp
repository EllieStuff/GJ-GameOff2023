// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameOff2023Character.h"
#include "Slimes/Slime.h"
#include "Slimes/SlimeProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

#include <Engine/World.h>


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AGameOff2023Character

AGameOff2023Character::AGameOff2023Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;


	/// Our code
	CurrSlimeType = (uint8)ESlimeType::JUMP;

	SlimesAmmunition.Add((uint8)ESlimeType::JUMP, INITIAL_AMMO);
	SlimesAmmunition.Add((uint8)ESlimeType::ICE, INITIAL_AMMO);
	SlimesAmmunition.Add((uint8)ESlimeType::METAL, INITIAL_AMMO);
	SlimesAmmunition.Add((uint8)ESlimeType::TEST, INITIAL_AMMO);

	AmmoColors.Add((uint8)ESlimeType::JUMP, FColor::Green);
	AmmoColors.Add((uint8)ESlimeType::ICE, FColor::Cyan);
	AmmoColors.Add((uint8)ESlimeType::METAL, FColor::Yellow);
	AmmoColors.Add((uint8)ESlimeType::TEST, FColor::White);

	Projectiles.Add((uint8)ESlimeType::JUMP, nullptr);
	Projectiles.Add((uint8)ESlimeType::ICE, nullptr);
	Projectiles.Add((uint8)ESlimeType::METAL, nullptr);
	Projectiles.Add((uint8)ESlimeType::TEST, nullptr);

}

void AGameOff2023Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGameOff2023Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind gun events
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGameOff2023Character::OnFirePressed);
	PlayerInputComponent->BindAction("SuckSlime", IE_Pressed, this, &AGameOff2023Character::OnSuckSlimePressed);
	PlayerInputComponent->BindAction("SuckSlime", IE_Released, this, &AGameOff2023Character::OnSuckSlimeReleased);
	PlayerInputComponent->BindAxis("ChangeAmmoType", this, &AGameOff2023Character::OnChangeSlimeAmmoType);
	//PlayerInputComponent->BindAction("ChangeAmmoUp", IE_Pressed, this, &AGameOff2023Character::OnChangeSlimeAmmoUp);
	//PlayerInputComponent->BindAction("ChangeAmmoDown", IE_Pressed, this, &AGameOff2023Character::OnChangeSlimeAmmoDown);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGameOff2023Character::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AGameOff2023Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameOff2023Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGameOff2023Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGameOff2023Character::LookUpAtRate);
}

void AGameOff2023Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InSuckSlimesMode) 
		UpdateSlimeSuckMode(DeltaTime);

}

bool AGameOff2023Character::CanShoot()
{
	return SlimeLoaded && SlimesAmmunition[CurrSlimeType] > 0;
}

void AGameOff2023Character::OnFirePressed()
{
	if (!CanShoot()) return;
	SlimeLoaded = false;
	RemoveSlimeAmmunition(CurrSlimeType, 1);
	ShootSlime();
	GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &AGameOff2023Character::LoadSlime, 1.0f);
}

void AGameOff2023Character::ShootSlime()
{
	// try and fire a projectile
	if (Projectiles[CurrSlimeType] != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				ASlimeProjectile* projectileRef = World->SpawnActor<ASlimeProjectile>(Projectiles[CurrSlimeType], SpawnLocation, SpawnRotation);
				
				if (projectileRef != nullptr) {
					FVector SpawnRotationEuler = SpawnRotation.Euler();
					SpawnRotationEuler.Y += 180;
					projectileRef->SetSlimeRotation(SpawnRotationEuler.Rotation());
				}
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				// Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				// Spawn the projectile at the muzzle
				ASlimeProjectile* projectileRef = World->SpawnActor<ASlimeProjectile>(Projectiles[CurrSlimeType], SpawnLocation, SpawnRotation, ActorSpawnParams);
				
				if (projectileRef != nullptr) {
					FVector SpawnRotationEuler = SpawnRotation.Euler();
					SpawnRotationEuler.Y += 180;
					projectileRef->SetSlimeRotation(SpawnRotationEuler.Rotation());
				}
			}
			
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AGameOff2023Character::LoadSlime()
{
	SlimeLoaded = true;
}

void AGameOff2023Character::OnSuckSlimePressed()
{
	InSuckSlimesMode = true;
	ResetSlimeSuckMode();
	SlimeSuckBeginEvent();
}

void AGameOff2023Character::OnSuckSlimeReleased()
{
	InSuckSlimesMode = false;
	ResetSlimeSuckMode();
	SlimeSuckEndEvent();
}

void AGameOff2023Character::ResetSlimeSuckMode()
{
	SuckSlimeTimer = 0;
	if (SlimeBeingSucked) {
		SlimeBeingSucked->Anim_IsBeingSucked = false;
		SlimeBeingSucked = nullptr;
	}
}

void AGameOff2023Character::UpdateSlimeSuckMode(float DeltaTime)
{
	ASlime* SlimeToSuck = GetSlimeToSuck();
	if (SlimeToSuck == nullptr || SlimeBeingSucked != SlimeToSuck) {
		if (SlimeBeingSucked) SlimeBeingSucked->Anim_IsBeingSucked = false;
		if (SlimeToSuck) {
			SlimeBeingSucked = SlimeToSuck;
			SlimeBeingSucked->Anim_IsBeingSucked = true;
		}
		SuckSlimeTimer = 0;
		UE_LOG(LogTemp, Warning, TEXT("SlimeSuckMode reseted or slime not found."));
	}
	else {
		if (SlimesAmmunition[(uint8)SlimeToSuck->GetSlimeType()] >= MAX_AMMO) return;

		SuckSlimeTimer += DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("Removing at: %s. Time Count = %f"), *SlimeToSuck->GetName(), SuckSlimeTimer);
		if (SuckSlimeTimer >= SuckSlimeDelay) {
			// Slime Ammo is added from SlimeReverseProjectile
			//AddSlimeAmmunition((uint8)SlimeToSuck->GetSlimeType(), 1);
			UE_LOG(LogTemp, Warning, TEXT("Slime Removed at: %s --> %i slimes remaining."), *SlimeToSuck->GetName(), SlimeToSuck->GetSlimeAmount() - 1);
			ResetSlimeSuckMode();
			SlimeToSuck->RemoveSlime();
		}
	}
}

ASlime* AGameOff2023Character::GetSlimeToSuck()
{
	const UWorld* world = GetWorld();
	if (!world) return nullptr;

	FVector eyesLocation;
	FRotator eyesRotation;
	this->GetActorEyesViewPoint(eyesLocation, eyesRotation);

	FVector rayStart = eyesLocation;
	FVector rayEnd = rayStart + eyesRotation.Vector() * 10000.0f;

	FHitResult hit;

	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);

	bool success = world->LineTraceSingleByChannel(hit, rayStart, rayEnd, ECC_GameTraceChannel2, queryParams);

	if (success) {
		AActor* otherActor = hit.GetActor();
		if (!otherActor) return nullptr;

		ASlime* targetSlime = Cast<ASlime>(otherActor);
		if (targetSlime) return targetSlime;
	}
	return nullptr;
}


void AGameOff2023Character::OnChangeSlimeAmmoType(float value)
{
	const float THRESHOLD = 0.2f;
	if (value > THRESHOLD) {
		CurrSlimeType++;
		if (CurrSlimeType >= (uint8)ESlimeType::COUNT) CurrSlimeType = 0;
		RefreshAmmunitionHUD();
		//const FString OnScreenMessage = FString::Printf(TEXT("Type is %i, ammunition is %i"), CurrSlimeType, SlimesAmmunition[CurrSlimeType]);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, OnScreenMessage);
	}
	else if (value < -THRESHOLD) {
		if (CurrSlimeType <= 0) CurrSlimeType = (uint8)ESlimeType::COUNT - 1;
		else CurrSlimeType--;
		RefreshAmmunitionHUD();
		//const FString OnScreenMessage = FString::Printf(TEXT("Type is %i, ammunition is %i"), CurrSlimeType, SlimesAmmunition[CurrSlimeType]);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, OnScreenMessage);
	}
}

void AGameOff2023Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGameOff2023Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFirePressed();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AGameOff2023Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AGameOff2023Character::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AGameOff2023Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AGameOff2023Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AGameOff2023Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGameOff2023Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AGameOff2023Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGameOff2023Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AGameOff2023Character::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGameOff2023Character::TouchUpdate);
		return true;
	}
	
	return false;
}

bool AGameOff2023Character::AddSlimeAmmunition(uint8 SlimeAmmunitionType, uint8 AmmunitionToAdd)
{
	if (!SlimesAmmunition.Find(SlimeAmmunitionType)) return false;

	SlimesAmmunition[SlimeAmmunitionType] += AmmunitionToAdd;
	RefreshAmmunitionPercentage();
	return true;
}

bool AGameOff2023Character::RemoveSlimeAmmunition(uint8 SlimeAmmunitionType, uint8 AmmunitionToRemove)
{
	if (!SlimesAmmunition.Find(SlimeAmmunitionType)) return false;

	SlimesAmmunition[SlimeAmmunitionType] -= AmmunitionToRemove;
	RefreshAmmunitionPercentage();
	return true;
}

float AGameOff2023Character::GetCurrSlimeAmmunitionPercentage()
{
	return (float)SlimesAmmunition[CurrSlimeType] / (float)MAX_AMMO;
}

FColor AGameOff2023Character::GetCurrSlimeAmmunitionColor()
{
	if (!AmmoColors.Contains(CurrSlimeType)) return FColor::Black;

	return AmmoColors[CurrSlimeType];
}
