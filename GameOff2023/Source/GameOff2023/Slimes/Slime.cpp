// Fill out your copyright notice in the Description page of Project Settings.


#include "Slimes/Slime.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Slimes/SlimeReverseProjectile.h"

// Sets default values
ASlime::ASlime()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;
	MeshComp->SetSimulatePhysics(true);
	//MeshComp->SetCollisionProfileName("Slime");
	//MeshComp->BodyInstance.bNotifyRigidBodyCollision = true;
	//MeshComp->SetGenerateOverlapEvents(true);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	//CollisionMesh->AddToRoot();
	//CollisionMesh->SetSimulatePhysics(true);
	CollisionMesh->SetCollisionProfileName("Slime");
	CollisionMesh->BodyInstance.bNotifyRigidBodyCollision = true;
	CollisionMesh->SetGenerateOverlapEvents(true);


	//MeshComp->SetAllBodiesCollisionObjectType(ECC_GameTraceChannel2);

	//GetMesh()->SetCollisionProfileName("NoCollision");

}

// Called when the game starts or when spawned
void ASlime::BeginPlay()
{
	Super::BeginPlay();

	//Start();
}

void ASlime::UpdateBehaviour()
{
	// Write particular behaviour of each Slime as an overrided method
}

void ASlime::UpdateBehaviourEvent_Implementation()
{
	UpdateBehaviour();
}

void ASlime::ActivateBehaviour()
{
	BehaviourActive = true;
}

void ASlime::ActivateBehaviourEvent_Implementation()
{
	ActivateBehaviour();
}

void ASlime::DeactivateBehaviour()
{
	BehaviourActive = true;
}

void ASlime::DeactivateBehaviourEvent_Implementation()
{
	DeactivateBehaviour();
}


void ASlime::RefreshTargetScale()
{
	TargetScale = BaseScale * (SlimeAmount / 2.0f + 0.5f);
	CurrScale = GetActorRelativeScale3D();
	TargetCollScale = BaseCollScale * (SlimeAmount / 2.0f + 0.5f);
	CurrCollScale = CollisionMesh->GetRelativeScale3D();

	SizeLerpTimer = 0;
	LerpingScale = true;
	Anim_IsGrowing = true;
}

void ASlime::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;

	if (Cast<ASlime>(OtherActor)) {
		ASlime* targetSlime = Cast<ASlime>(OtherActor);
		if (targetSlime->GetSlimeType() == this->GetSlimeType()) {
			if (targetSlime->GetSlimeAmount() > this->GetSlimeAmount()) {
				targetSlime->AddSlime(this->GetSlimeAmount());
				this->Destroy();
			}
			else {
				this->AddSlime(targetSlime->GetSlimeAmount());
				targetSlime->Destroy();
			}
		}
		else {
			// Hacer que rebote?
		}
	}
	else {
		if (OtherActor->ActorHasTag("Floor")) {
			if (HasFloorBelow()) OnOverlapFloorEvent(OtherActor);
			else OnOverlapIncompatibleTerrain();
		}

		if (OtherActor->ActorHasTag("Water"))
			OnOverlapWaterEvent(OtherActor);

		//if (OtherActor->ActorHasTag("Wall"))
		//	OnOverlapIncompatibleTerrain();

	}

}


void ASlime::OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;
	//
	//if (Cast<ASlime>(OtherActor)) {
	//	ASlime* targetSlime = Cast<ASlime>(OtherActor);
	//	if (targetSlime->GetSlimeType() == this->GetSlimeType() && targetSlime->GetSlimeAmount() + this->GetSlimeAmount() <= targetSlime->MAX_SLIMES) {
	//		if (targetSlime->GetSlimeAmount() > this->GetSlimeAmount()) {
	//			targetSlime->AddSlime(this->GetSlimeAmount());
	//			this->Destroy();
	//		}
	//		else {
	//			this->AddSlime(targetSlime->GetSlimeAmount());
	//			targetSlime->Destroy();
	//		}
	//	}
	//	else {
	//		// Hacer que rebote o simplemente dejarle ser?
	//	}
	//}
	//else {
	//	if (OtherActor->ActorHasTag("Floor")) 
	//		OnOverlapFloorEvent(OtherActor);

	//	if (OtherActor->ActorHasTag("Water"))
	//		OnOverlapWaterEvent(OtherActor);
	//}
}

void ASlime::OnOverlapFloor(AActor* OtherActor)
{
	MeshComp->SetSimulatePhysics(false);
	CollisionMesh->SetSimulatePhysics(false);
	
	// Implement particular methods in each slime, if not using events
}

void ASlime::OnOverlapFloorEvent_Implementation(AActor* OtherActor)
{
	OnOverlapFloor(OtherActor);
}

void ASlime::OnOverlapWater(AActor* OtherActor)
{
	OnOverlapIncompatibleTerrain();

	// Implement particular methods in each slime, if not using events
}

void ASlime::OnOverlapWaterEvent_Implementation(AActor* OtherActor)
{
	OnOverlapWater(OtherActor);
}

void ASlime::OnOverlapIncompatibleTerrain()
{
	SpawnReverseProjectile();
	DestroySlimeEvent();
}

// Called every frame
void ASlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FirstFrame) {
		FirstFrame = false;
		Start();
	}


	if (LerpingScale) {
		SizeLerpTimer += DeltaTime;
		SetActorRelativeScale3D(FMath::Lerp<FVector>(CurrScale, TargetScale, SizeLerpTimer / SizeLerpDuration));
		CollisionMesh->SetRelativeScale3D(FMath::Lerp<FVector>(CurrCollScale, TargetCollScale, SizeLerpTimer / SizeLerpDuration));
		if (SizeLerpTimer >= SizeLerpDuration) {
			LerpingScale = false;
			Anim_IsGrowing = false;
			CurrScale = TargetScale;
			CurrCollScale = TargetCollScale;
		}
	}

	CollisionMesh->SetWorldLocation(this->GetActorLocation());
	//CollisionMesh->UpdateCollisionProfile();

	if(BehaviourActive) UpdateBehaviourEvent();

}


void ASlime::PlayAnimation(UAnimationAsset* Animation, bool Loops)
{
	MeshComp->PlayAnimation(Animation, Loops);
}

void ASlime::AddSlime(uint8 SlimesToAdd)
{
	SlimeAmount += SlimesToAdd;
	if (SlimeAmount > MAX_SLIMES) {
		SlimeAmount = MAX_SLIMES;
		UE_LOG(LogTemp, Warning, TEXT("Slimes Max Amount reached!"));
		// Hacer lo que sea, tal vez que rebote?
	}

	IncreaseSizeEvent();
}

void ASlime::IncreaseSizeFeedback()
{
	RefreshTargetScale();

	// Implement particular methods in each slime, if not using events
}

void ASlime::IncreaseSizeEvent_Implementation()
{
	IncreaseSizeFeedback();
}

void ASlime::RemoveSlime(uint8 SlimesToRemove)
{
	SlimeAmount -= SlimesToRemove;
	if (SlimeAmount < MIN_SLIMES) {
		SlimeAmount = 0;
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ASlime::DestroySlimeEvent, 1.0f);
	}
	SpawnReverseProjectile();
	DecreaseSizeEvent();

}

void ASlime::SpawnReverseProjectile()
{
	UWorld* const World = GetWorld();
	if (!World) return;

	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (SlimeReverseProjectile)
		World->SpawnActor<ASlimeReverseProjectile>(SlimeReverseProjectile, SpawnLocation, SpawnRotation, ActorSpawnParams);
	else
		UE_LOG(LogTemp, Error, TEXT("Slime not found!"));

}

bool ASlime::HasFloorBelow()
{
	const UWorld* world = GetWorld();
	if (!world) return false;

	FHitResult hit;
	FVector rayStart = GetActorLocation();
	FVector rayEnd = rayStart + FVector::DownVector * 25.0f;
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);

	bool success = world->LineTraceSingleByChannel(hit, rayStart, rayEnd, ECollisionChannel::ECC_WorldStatic, queryParams);
	return success;
}

void ASlime::Start()
{
	//Mesh->SetSimulatePhysics(true);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ASlime::OnOverlapBegin);
	CollisionMesh->OnComponentHit.AddDynamic(this, &ASlime::OnHitBegin);

	BaseScale = CurrScale = TargetScale = GetActorRelativeScale3D();
	BaseCollScale = CurrCollScale = TargetCollScale = CollisionMesh->GetRelativeScale3D();
	AppearEvent();
}

void ASlime::DecreaseSizeFeedback()
{
	RefreshTargetScale();

	// Implement particular methods in each slime, if not using events
}

void ASlime::DecreaseSizeEvent_Implementation()
{
	DecreaseSizeFeedback();
}

void ASlime::Appear()
{
	SetActorRelativeScale3D(FVector::OneVector / 2.0f);
	RefreshTargetScale();
	ActivateBehaviourEvent();
}

void ASlime::AppearEvent_Implementation()
{
	Appear();
}

void ASlime::DestroySlime()
{
	if (this == nullptr) return;
	GetWorldTimerManager().ClearTimer(TimerHandle);
	this->Destroy(); 
}

void ASlime::DestroySlimeEvent_Implementation()
{
	DestroySlime();
}

