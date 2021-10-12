// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FirstPersonProjectHUD.generated.h"

UCLASS()
class AFirstPersonProjectHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFirstPersonProjectHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

