// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Slime.generated.h"


UENUM(BlueprintType)
enum class ESlimeType : uint8
{
	JUMP UMETA(DisplayName = "Jump"),
	ICE UMETA(DisplayName = "Ice"),
	METAL UMETA(DisplayName = "Metal"),
	NOT_INITIALIZED UMETA(DisplayName = "Not initialized")
};

UCLASS()
class GAMEOFF2023_API ASlime : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlime();

protected:
	virtual void BeginPlay() override;

	const uint8 MAX_SLIMES = 10;
	const uint8 MIN_SLIMES = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	int SlimeAmount = 1;

	FVector baseScale, currScale, targetScale;
	float sizeLerpTimer = 0, sizeLerpDuration = 1;
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
	void ActivateBehaviour();
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void ActivateBehaviourEvent();
	void ActivateBehaviourEvent_Implementation();

	UFUNCTION(BlueprintCallable, Category = Slime)
	void DeactivateBehaviour();
	UFUNCTION(BlueprintNativeEvent, Category = Slime)
	void DeactivateBehaviourEvent();
	void DeactivateBehaviourEvent_Implementation();

/// Overlap Begin
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = Slime)
	void PlayAnimation(UAnimationAsset* Animation, bool Loops);

	UFUNCTION(BlueprintCallable, Category = Slime)
	void AddSlime(uint8 SlimesToAdd = 1);

	UFUNCTION(BlueprintCallable, Category = Slime)
	virtual void RemoveSlime(uint8 SlimesToRemove = 1);

	UFUNCTION(BlueprintCallable, Category = Slime)
	FORCEINLINE ESlimeType GetSlimeType() { return SlimeType; }

};
