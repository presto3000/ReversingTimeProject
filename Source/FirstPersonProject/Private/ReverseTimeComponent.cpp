// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseTimeComponent.h"

#include "FirstPersonProject/FirstPersonProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UReverseTimeComponent::UReverseTimeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bReversingTime = false;

	
	// ...
}


// Called when the game starts
void UReverseTimeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AFirstPersonProjectCharacter* RevTimeCharacter = Cast<AFirstPersonProjectCharacter> (UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	RevTimeCharacter->ReverseTimeDelegate.AddDynamic(this, &UReverseTimeComponent::SetReversingTime);

	
}

void UReverseTimeComponent::SetReversingTime(bool InReversingTime)
{
	bReversingTime = InReversingTime;

	if(InReversingTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Reversing Time!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Stop Reversing Time"));
	}
}

// Called every frame
void UReverseTimeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if(!bReversingTime) //storing data
	{
		RunningTime = 0.f;
		LeftReverseRunningTime = 0.f;
		RightReverseRunningTime = 0.f;
		
		AActor* Owner = GetOwner();
		//Geting a ref to a blueprint type 
		TInlineComponentArray<UStaticMeshComponent*> StaticMeshComponents;
		Owner->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
		if(StaticMeshComponents.Num() > 0)
		{
			UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(StaticMeshComponents[0]);
			if(SMC)
			{
				FFramePackage Package(Owner->GetActorLocation(), Owner->GetActorRotation(), SMC->GetPhysicsLinearVelocity(),
										SMC->GetPhysicsAngularVelocityInDegrees(), DeltaTime);

				if(RecordedTime < 15.0f)
				{
					StoredFrames.AddTail(Package);
					RecordedTime += Package.DeltaTime;
					bOutOfData = false;				
				}
				else
				{
					while(RunningTime >= 15.0f)
					{
						auto Head = StoredFrames.GetHead();
						float HeadDT = Head->GetValue().DeltaTime;
						StoredFrames.RemoveNode(Head);
						RecordedTime -= HeadDT;
					}
					StoredFrames.AddTail(Package);
					RecordedTime += Package.DeltaTime;
					bOutOfData = false;
				}
			}	
		}

		
	}
	else if(!bOutOfData) //reversing time
	{
		RunningTime += DeltaTime * .2f;
		auto Right = StoredFrames.GetTail();
		auto Left = Right->GetPrevNode();
		LeftReverseRunningTime = RightReverseRunningTime + Right->GetValue().DeltaTime;
		while(RunningTime > LeftReverseRunningTime)
		{
			RightReverseRunningTime += Right->GetValue().DeltaTime;
			Right = Left;
			LeftReverseRunningTime += Left->GetValue().DeltaTime;
			Left = Left->GetPrevNode();

			auto Tail = StoredFrames.GetTail();
			RecordedTime -= Tail->GetValue().DeltaTime;
			StoredFrames.RemoveNode(Tail);
			if(Left == StoredFrames.GetHead())
			{
				bOutOfData = true;
			}
	
		}
		// now Left and Right surround RunningTime
		if(RunningTime <= LeftReverseRunningTime && RunningTime >= RightReverseRunningTime)
		{
			float DT = RunningTime - RightReverseRunningTime;
			float Interval = LeftReverseRunningTime - RightReverseRunningTime;
			float Fraction = DT / Interval;

			FVector InterpLoc = FMath::VInterpTo(Right->GetValue().Location, Left->GetValue().Location, Fraction, 1.0f);
			FRotator InterpRot = FMath::RInterpTo(Right->GetValue().Rotation, Left->GetValue().Rotation, Fraction, 1.0f);
			FVector InterpLinVel = FMath::VInterpTo(Right->GetValue().LinearVelocity, Left->GetValue().LinearVelocity, Fraction, 1.0f);
			FVector InterpAngVel = FMath::VInterpTo(Right->GetValue().AngularVelocity, Left->GetValue().AngularVelocity, Fraction, 1.0f);

			SetActorVariables(InterpLoc, InterpRot, InterpLinVel, InterpAngVel);
		}
	}
}

void UReverseTimeComponent::SetActorVariables(FVector Location, FRotator Rotation,
	FVector LinearVel, FVector AngularVel)
{

		AActor* Owner = GetOwner();
		Owner->SetActorLocation(Location);
		Owner->SetActorRotation(Rotation);
			
		TInlineComponentArray<UStaticMeshComponent*> StaticMeshComponents;
		Owner->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
			
		if(StaticMeshComponents.Num() > 0)
		{
			UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(StaticMeshComponents[0]);
			if(SMC)
			{
				SMC->SetPhysicsLinearVelocity(LinearVel);
				SMC->SetPhysicsAngularVelocityInDegrees(AngularVel);
					
			}
		}
	
}

