#pragma once

#include "Math/Vector.h"
#include "Math/Rotator.h"
#include "Math/Vector2D.h"
#include "FramePackagee.generated.h"

USTRUCT()
struct FFramePackage
{
	GENERATED_BODY()
	FORCEINLINE FFramePackage();
	FORCEINLINE FFramePackage(FVector InLocation, FRotator InRotation, FVector InLinearVelocity, FVector InAngularVelocity, float InDeltaTime);
	FVector Location;
	FRotator Rotation;
	FVector LinearVelocity;
	FVector AngularVelocity;

	bool bIsPlayerPawn;
	FVector2D MouseXY;

	float DeltaTime;
	
};

FFramePackage::FFramePackage()
{
	
}

FFramePackage::FFramePackage(FVector InLocation, FRotator InRotation, FVector InLinearVelocity,FVector InAngularVelocity,
	 float InDeltaTime) :
	Location(InLocation), Rotation(InRotation), LinearVelocity(InLinearVelocity), AngularVelocity(InAngularVelocity),
	DeltaTime(InDeltaTime)
{
}
