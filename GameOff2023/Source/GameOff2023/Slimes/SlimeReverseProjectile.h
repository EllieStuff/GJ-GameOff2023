// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slime.h"
#include "GameFramework/Actor.h"
#include "SlimeReverseProjectile.generated.h"

UCLASS()
class GAMEOFF2023_API ASlimeReverseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlimeReverseProjectile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	ESlimeType SlimeType { ESlimeType::NOT_INITIALIZED };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Defaults)
	UStaticMeshComponent* MeshComp;

	class AGameOff2023Character* PlayerRef { nullptr };

private:
	FVector InitialLocation { FVector::ZeroVector };

	UPROPERTY(EditAnywhere, Category = Defaults)
	float LerpTime = 1;
	float Timer = 0;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddAmmoToPlayer();

};
