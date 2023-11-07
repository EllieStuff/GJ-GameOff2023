// Fill out your copyright notice in the Description page of Project Settings.


#include "Slimes/SlimeProjectile.h"
#include "Slimes/Slime.h"
//#include "Slimes/TestSlime.h"

ASlimeProjectile::ASlimeProjectile() : AGameOff2023Projectile()
{
	SlimeToSpawnType = (uint8)(ESlimeType::TEST);
}


void ASlimeProjectile::SetSlimeType(uint8 SlimeType)
{
	SlimeToSpawnType = SlimeType;
	switch ((ESlimeType)(SlimeType))
	{
	case ESlimeType::JUMP:
		
		break;

	case ESlimeType::ICE:

		break;

	case ESlimeType::METAL:

		break;

	case ESlimeType::TEST:
		// ToDo: Mirar si això funciona
		//SlimeToSpawn = LoadClass<ATestSlime>(NULL, TEXT("Blueprint'/Game/Blueprints/Slimes/bpSlimeProjectile.bpSlimeProjectile_C'"), NULL, LOAD_None, NULL);
		break;


	default:
		UE_LOG(LogTemp, Warning, TEXT("Slime type not contemplated."));
		break;
	}
}

void ASlimeProjectile::SpawnSlime()
{
	UWorld* const World = GetWorld();
	if (!World) return;

	/*const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	World->SpawnActor<ASlime>(SlimeToSpawn, SpawnLocation, SpawnRotation);*/
	
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	const FVector SpawnLocation = GetActorLocation();
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	World->SpawnActor<ASlime>(SlimeToSpawn, SpawnLocation, SpawnRotation, ActorSpawnParams);
	
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

