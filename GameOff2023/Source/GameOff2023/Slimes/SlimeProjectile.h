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

	FRotator SpawnRotation { FRotator::ZeroRotator };

public:
	ASlimeProjectile();

	UPROPERTY(EditAnywhere, Category = Components)
	TSubclassOf<class ASlime> SlimeToSpawn { nullptr };
	UPROPERTY(EditAnywhere, Category = Components)
	uint8 SlimeToSpawnType;

	//UPROPERTY(EditAnywhere, Category = Components)
	//TMap<uint8, TSubclassOf<class ASlime>> SlimesToSpawn;
	//uint8 SlimeToSpawnType;

	/*UPROPERTY(EditAnywhere, Category = Components)
	UStaticMesh* MeshComp;
	UPROPERTY(EditAnywhere, Category = Components)
	TMap<uint8, UMaterialInterface*> SlimesToSpawn_Materials;*/

	//void SetSlimeType(uint8 SlimeType);
	void SetSlimeRotation(FRotator Rotation);
	void SpawnSlime();

	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
