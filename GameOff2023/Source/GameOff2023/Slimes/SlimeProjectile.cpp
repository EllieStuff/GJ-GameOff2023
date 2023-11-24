// Fill out your copyright notice in the Description page of Project Settings.


#include "Slimes/SlimeProjectile.h"

#include "Slimes/Slime.h"
#include "Slimes/TestSlime.h"
#include "Slimes/MetalSlime.h"


ASlimeProjectile::ASlimeProjectile() : AGameOff2023Projectile()
{
	//SlimeToSpawnType = (uint8)(ESlimeType::TEST);

	/*SlimesToSpawn.Add((uint8)ESlimeType::JUMP, nullptr);
	SlimesToSpawn.Add((uint8)ESlimeType::ICE, nullptr);
	SlimesToSpawn.Add((uint8)ESlimeType::METAL, nullptr);
	SlimesToSpawn.Add((uint8)ESlimeType::TEST, nullptr);*/

	/*SlimesToSpawn_Materials.Add((uint8)ESlimeType::JUMP, nullptr);
	SlimesToSpawn_Materials.Add((uint8)ESlimeType::ICE, nullptr);
	SlimesToSpawn_Materials.Add((uint8)ESlimeType::METAL, nullptr);
	SlimesToSpawn_Materials.Add((uint8)ESlimeType::TEST, nullptr);*/
}


//void ASlimeProjectile::SetSlimeType(uint8 SlimeType)
//{
//	if (this != nullptr) {
//		SlimeToSpawnType = SlimeType;
//		if (!MeshComp) {
//			UE_LOG(LogTemp, Error, TEXT("MeshComp not assigned"));
//			return;
//		}
//
//		MeshComp->SetMaterial(0, SlimesToSpawn_Materials[SlimeType]);
//	}
//}

void ASlimeProjectile::SetSlimeRotation(FRotator Rotation)
{
	/*if (this != nullptr) {
		SpawnRotation = Rotation;
	}*/
}

void ASlimeProjectile::SpawnSlime()
{
	UWorld* const World = GetWorld();
	if (!World) return;

	/*const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	World->SpawnActor<ASlime>(SlimeToSpawn, SpawnLocation, SpawnRotation);*/
	
	const FVector SpawnLocation = GetActorLocation();
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//if(SlimesToSpawn.Contains(SlimeToSpawnType))
	//	World->SpawnActor<ASlime>(SlimesToSpawn[SlimeToSpawnType], SpawnLocation, SpawnRotation, ActorSpawnParams);
	if(SlimeToSpawn)
		World->SpawnActor<ASlime>(SlimeToSpawn, SpawnLocation, SpawnRotation, ActorSpawnParams);
	else
		UE_LOG(LogTemp, Error, TEXT("Slime not found!"));
	
}

void ASlimeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor == nullptr) || (OtherActor == this) || (OtherComp == nullptr)) return;

	if (Cast<ASlime>(OtherActor))
	{
		ASlime* targetSlime = Cast<ASlime>(OtherActor);
		if (targetSlime->GetSlimeType() == (ESlimeType)(SlimeToSpawnType) && targetSlime->GetSlimeAmount() < targetSlime->MAX_SLIMES) {
			targetSlime->AddSlime();
			Destroy();
		}
		else {
			UPrimitiveComponent* thisComp = Cast<UPrimitiveComponent>(this);
			if(thisComp)
				thisComp->AddImpulseAtLocation(GetVelocity() * -BounceForce, GetActorLocation());
			//HitComp->AddImpulseAtLocation(GetVelocity() * -BounceForce, GetActorLocation());
		}

	}
	else {
		SpawnSlime();
		Destroy();
	}
	

}

