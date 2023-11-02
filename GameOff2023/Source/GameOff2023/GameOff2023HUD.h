// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameOff2023HUD.generated.h"

UCLASS()
class AGameOff2023HUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameOff2023HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

