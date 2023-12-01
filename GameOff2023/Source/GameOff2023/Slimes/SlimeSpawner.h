// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlimeSpawner.generated.h"

UCLASS()
class GAMEOFF2023_API ASlimeSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlimeSpawner();

	UPROPERTY(EditDefaultsOnly, Category = Spawner)
	TSubclassOf<class ASlime> SlimeToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
	float SpawnYRotation = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
