// Fill out your copyright notice in the Description page of Project Settings.


#include "Slimes/SlimeReverseProjectile.h"

#include "GameOff2023Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASlimeReverseProjectile::ASlimeReverseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

}

// Called when the game starts or when spawned
void ASlimeReverseProjectile::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();

	if (!GetWorld()) return;
	PlayerRef = dynamic_cast<AGameOff2023Character*>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!PlayerRef) return;
	
}

// Called every frame
void ASlimeReverseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Timer >= LerpTime) {
		AddAmmoToPlayer();
		this->Destroy();
	}
	else {
		Timer += DeltaTime;
		const FVector nextLocation = FMath::Lerp<FVector>(InitialLocation, PlayerRef->GetActorLocation(), Timer / LerpTime);
		SetActorLocation(nextLocation);
	}

}

void ASlimeReverseProjectile::AddAmmoToPlayer()
{
	PlayerRef->AddSlimeAmmunition((uint8)SlimeType);
}

