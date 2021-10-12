// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FramePackagee.h"
#include "ReverseTimeComponent.generated.h"


struct FFramePackage;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class FIRSTPERSONPROJECT_API UReverseTimeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReverseTimeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetReversingTime(bool InReversingTime);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
//=====================================
	// Reversing time when true, collecting data when false
	bool bReversingTime;
	// Out of time data, cannot keep reversing
	bool bOutOfData;
	// Actual time since we started reversing time
	float RunningTime;
	// Running count of the Frame Package Delta Times
	float LeftReverseRunningTime;
	float RightReverseRunningTime;
	// Total amount of time recorded in FramePackage
	float RecordedTime;

	// Double LinkList Container
	TDoubleLinkedList<FFramePackage>StoredFrames;

	void SetActorVariables(FVector Location, FRotator Rotation, FVector LinearVel, FVector AngularVel);
	
	
};
