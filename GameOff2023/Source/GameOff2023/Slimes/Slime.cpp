// Fill out your copyright notice in the Description page of Project Settings.


#include "Slimes/Slime.h"

// Sets default values
ASlime::ASlime()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	baseScale = currScale = targetScale = GetActorRelativeScale3D();
}

// Called when the game starts or when spawned
void ASlime::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ASlime::OnOverlapBegin);
	
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
	targetScale = baseScale * (SlimeAmount / 2.0f + 0.5f);
	LerpingScale = true;
}

void ASlime::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ASlime>(OtherActor)) {
		ASlime* targetSlime = Cast<ASlime>(OtherActor);
		if (targetSlime->GetSlimeType() == GetSlimeType()) {
			AddSlime();
		}
		else {
			// Hacer que rebote?
		}
	}
	else {
		OnOverlapScenarioEvent(OtherActor);
	}

}

void ASlime::OnOverlapScenario(AActor* OtherActor)
{
	RefreshTargetScale();
	ActivateBehaviourEvent();

	// Implement particular methods in each slime, if not using events
}

void ASlime::OnOverlapScenarioEvent_Implementation(AActor* OtherActor)
{
	OnOverlapScenario(OtherActor);
}

// Called every frame
void ASlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LerpingScale) {
		sizeLerpTimer += DeltaTime;
		SetActorRelativeScale3D(FMath::Lerp<FVector>(currScale, targetScale, sizeLerpTimer / sizeLerpDuration));
		if (sizeLerpTimer >= sizeLerpDuration) {
			LerpingScale = false;
			currScale = targetScale;
		}
	}

	if(BehaviourActive) UpdateBehaviourEvent();

}


void ASlime::PlayAnimation(UAnimationAsset* Animation, bool Loops)
{
	Mesh->PlayAnimation(Animation, Loops);
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
		this->Destroy();
	}
	else {
		DecreaseSizeFeedback();
	}

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

