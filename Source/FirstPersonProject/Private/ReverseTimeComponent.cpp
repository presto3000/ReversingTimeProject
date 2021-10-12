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
		auto Tail = StoredFrames.GetTail();
		if(Tail)
		{
			AActor* Owner = GetOwner();
			Owner->SetActorLocation(Tail->GetValue().Location);
			Owner->SetActorRotation(Tail->GetValue().Rotation);
			
			TInlineComponentArray<UStaticMeshComponent*> StaticMeshComponents;
			Owner->GetComponents<UStaticMeshComponent>(StaticMeshComponents);
			
			if(StaticMeshComponents.Num() > 0)
			{
				UStaticMeshComponent* SMC = Cast<UStaticMeshComponent>(StaticMeshComponents[0]);
				if(SMC)
				{
					SMC->SetPhysicsLinearVelocity(Tail->GetValue().LinearVelocity);
					SMC->SetPhysicsAngularVelocityInDegrees(Tail->GetValue().AngularVelocity);
					
				}
			}
			auto Head = StoredFrames.GetHead();
			if(Head == Tail)
			{
				RecordedTime = 0.f;
				bOutOfData = true;
			}
			else
			{
				RecordedTime -= Tail->GetValue().DeltaTime;
			}
			StoredFrames.RemoveNode(Tail);
		}
	}
}

