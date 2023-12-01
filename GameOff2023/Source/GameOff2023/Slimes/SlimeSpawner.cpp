// Fill out your copyright notice in the Description page of Project Settings.


#include "Slimes/SlimeSpawner.h"
#include "Slimes/Slime.h"



// Sets default values
ASlimeSpawner::ASlimeSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASlimeSpawner::BeginPlay()
{
	Super::BeginPlay();

	const FVector SpawnRotation = FVector(0, SpawnYRotation, 0);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<ASlime>(SlimeToSpawn, GetActorLocation(), SpawnRotation.Rotation(), ActorSpawnParams);
	
}

// Called every frame
void ASlimeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

