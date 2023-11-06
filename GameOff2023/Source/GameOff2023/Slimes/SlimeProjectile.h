// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameOff2023Projectile.h"
#include "SlimeProjectile.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFF2023_API ASlimeProjectile : public AGameOff2023Projectile
{
	GENERATED_BODY()

public:
	ASlimeProjectile();

	UPROPERTY(EditAnywhere, Category = Components)
	TSubclassOf<class ASlime> SlimeToSpawn;
	uint8 SlimeToSpawnType;

	void SetSlimeType(uint8 SlimeType);
	void SpawnSlime();

	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
};