// Fill out your copyright notice in the Description page of Project Settings.


#include "Slimes/Slime.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ASlime::ASlime()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);

	BaseScale = CurrScale = TargetScale = GetActorRelativeScale3D();
}

// Called when the game starts or when spawned
void ASlime::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ASlime::OnOverlapBegin);
	StaticMesh->OnComponentHit.AddDynamic(this, &ASlime::OnHitBegin);
	StaticMesh->SetSimulatePhysics(true);
	//Mesh->OnComponentBeginOverlap.AddDynamic(this, &ASlime::OnOverlapBegin);
	//Mesh->SetSimulatePhysics(true);
	
	AppearEvent();
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
	SizeLerpTimer = 0;
	LerpingScale = true;
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
		OnOverlapScenarioEvent(OtherActor);
	}

}

//void ASlime::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (Cast<ASlime>(OtherActor)) {
//		ASlime* targetSlime = Cast<ASlime>(OtherActor);
//		if (targetSlime->GetSlimeType() == GetSlimeType()) {
//			AddSlime();
//		}
//		else {
//			// Hacer que rebote?
//		}
//	}
//	else {
//		OnOverlapScenarioEvent(OtherActor);
//	}
//}

void ASlime::OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;

	if (Cast<ASlime>(OtherActor)) {
		ASlime* targetSlime = Cast<ASlime>(OtherActor);
		if (targetSlime->GetSlimeType() == this->GetSlimeType() && targetSlime->GetSlimeAmount() + this->GetSlimeAmount() <= targetSlime->MAX_SLIMES) {
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
			// Hacer que rebote o simplemente dejarle ser?
		}
	}
	else {
		OnOverlapScenarioEvent(OtherActor);
	}
}

void ASlime::OnOverlapScenario(AActor* OtherActor)
{
	// ToDo: Buscar alternativa que no sigui el nom... 
	//	- no se perque tags no funcionen
	//if (OtherActor->ActorHasTag("Floor")) {
	if (OtherActor->GetName().Contains("Floor")) {
		StaticMesh->SetSimulatePhysics(false);
	}

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
		SizeLerpTimer += DeltaTime;
		SetActorRelativeScale3D(FMath::Lerp<FVector>(CurrScale, TargetScale, SizeLerpTimer / SizeLerpDuration));
		if (SizeLerpTimer >= SizeLerpDuration) {
			LerpingScale = false;
			CurrScale = TargetScale;
		}
	}

	Mesh->UpdateCollisionProfile();

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
		SlimeAmount = 0;
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ASlime::DestroySlimeEvent, 1.0f);
	}
	DecreaseSizeEvent();

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

