// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Slimes/Slime.h"
#include "TestSlime.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFF2023_API ATestSlime : public ASlime
{
	GENERATED_BODY()

public:
	ATestSlime();

	void Tick(float DelataTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	UMeshComponent* StaticMesh;

protected:
	void BeginPlay() override;

	void ActivateBehaviour() override;
	void UpdateBehaviour() override;
	void DeactivateBehaviour() override;

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnOverlapScenario(AActor* OtherActor) override;

	void IncreaseSizeFeedback() override;
	void DecreaseSizeFeedback() override;
	
};
