// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slime.generated.h"


UENUM(BlueprintType)
enum class ESlimeType : uint8
{
	JUMP = 0 UMETA(DisplayName = "Jump"),
	ICE = 1 UMETA(DisplayName = "Ice"),
	METAL = 2 UMETA(DisplayName = "Metal"),
	COUNT UMETA(DisplayName = "Count"),
	NOT_INITIALIZED UMETA(DisplayName = "Not initialized"),
	TEST UMETA(DisplayName = "Test")
};

UCLASS()
class GAMEOFF2023_API ASlime : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlime();

	const int MAX_SLIMES = 10;
	const int MIN_SLIMES = 1;

protected:
	virtual void BeginPlay() override;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	int SlimeAmount = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	USkeletalMeshComponent* Mesh;

	/// Mesh de uso temporal hasta que tengamos los modelos riggeados, usar el USKeletalMeshComponent de arriba cuando esten
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	UStaticMeshComponent* StaticMesh;
	///

	FVector BaseScale, CurrScale, TargetScale;
	float SizeLerpTimer = 0, SizeLerpDuration = 1;
	bool LerpingScale = false;

	bool BehaviourActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	ESlimeType SlimeType { ESlimeType::NOT_INITIALIZED };


/// Behaviours
	UFUNCTION(BlueprintCallable, Category = Slime)
	virtual void UpdateBehaviour();
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void UpdateBehaviourEvent();
	void UpdateBehaviourEvent_Implementation();

	UFUNCTION(BlueprintCallable, Category = Slime)
	virtual void ActivateBehaviour();
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void ActivateBehaviourEvent();
	void ActivateBehaviourEvent_Implementation();

	UFUNCTION(BlueprintCallable, Category = Slime)
	virtual void DeactivateBehaviour();
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void DeactivateBehaviourEvent();
	void DeactivateBehaviourEvent_Implementation();

/// Overlap Begin
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	/*UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
	UFUNCTION()
	virtual void OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Overlap with Scenario
	UFUNCTION(BlueprintCallable, Category = Slime)
	virtual void OnOverlapScenario(AActor* OtherActor);
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void OnOverlapScenarioEvent(AActor* OtherActor);
	void OnOverlapScenarioEvent_Implementation(AActor* OtherActor);

/// Change Scale
	UFUNCTION(BlueprintCallable, Category = Slime)
	void RefreshTargetScale();

	// Increase Scale
	UFUNCTION(BlueprintCallable, Category = Slime)
	virtual void IncreaseSizeFeedback();
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void IncreaseSizeEvent();
	void IncreaseSizeEvent_Implementation();

	// Decrease Scale
	UFUNCTION(BlueprintCallable, Category = Slime)
	virtual void DecreaseSizeFeedback();
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void DecreaseSizeEvent();
	void DecreaseSizeEvent_Implementation();

/// Spawning & Despawning
	UFUNCTION(BlueprintCallable, Category = Slime)
	virtual void Appear();
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void AppearEvent();
	void AppearEvent_Implementation();

	UFUNCTION(BlueprintCallable, Category = Slime)
	virtual void DestroySlime();
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void DestroySlimeEvent();
	void DestroySlimeEvent_Implementation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Slime)
	void PlayAnimation(UAnimationAsset* Animation, bool Loops);

	UFUNCTION(BlueprintCallable, Category = Slime)
	void AddSlime(uint8 SlimesToAdd = 1);

	UFUNCTION(BlueprintCallable, Category = Slime)
	void RemoveSlime(uint8 SlimesToRemove = 1);

	UFUNCTION(BlueprintCallable, Category = Slime)
	FORCEINLINE ESlimeType GetSlimeType() { return SlimeType; }
	
	UFUNCTION(BlueprintCallable, Category = Slime)
	FORCEINLINE int GetSlimeAmount() { return SlimeAmount; }

};
