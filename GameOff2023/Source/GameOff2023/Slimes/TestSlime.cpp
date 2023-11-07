// Fill out your copyright notice in the Description page of Project Settings.


#include "Slimes/TestSlime.h"


ATestSlime::ATestSlime() : ASlime()
{
	SlimeType = ESlimeType::TEST;

}

void ATestSlime::BeginPlay()
{
	Super::BeginPlay();
}

void ATestSlime::Tick(float DelataTime)
{
	Super::Tick(DelataTime);
}


/// Behaviours
void ATestSlime::ActivateBehaviour()
{
	Super::ActivateBehaviour();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Effect started!"));
}

void ATestSlime::UpdateBehaviour()
{
	Super::UpdateBehaviour();
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Effect updated!"));
}

void ATestSlime::DeactivateBehaviour()
{
	Super::DeactivateBehaviour();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Effect ended!"));
}


/// Collisions
void ATestSlime::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Something Overlapped!"));
}

void ATestSlime::OnOverlapScenario(AActor* OtherActor)
{
	Super::OnOverlapScenario(OtherActor);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Scenario Overlapped!"));
}


/// Feedback
void ATestSlime::IncreaseSizeFeedback()
{
	Super::IncreaseSizeFeedback();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Size Increased!"));
}

void ATestSlime::DecreaseSizeFeedback()
{
	Super::DecreaseSizeFeedback();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Size decreased!"));
}

void ATestSlime::Appear()
{
	Super::Appear();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Slime Appeared!"));
}

void ATestSlime::DestroySlime()
{
	// Code here, before Super function
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Slime Destroyed!"));

	Super::DestroySlime();
}

