// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameOff2023Projectile.h"
#include "SlimeProjectile.generated.h"

//#include <List>

/**
 * 
 */
UCLASS()
class GAMEOFF2023_API ASlimeProjectile : public AGameOff2023Projectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	float BounceForce = 100.0f;

public:
	ASlimeProjectile();

	UPROPERTY(EditAnywhere, Category = Components)
	TMap<uint8, TSubclassOf<class ASlime>> SlimesToSpawn;
	uint8 SlimeToSpawnType;

	void SetSlimeType(uint8 SlimeType);
	void SpawnSlime();

	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
