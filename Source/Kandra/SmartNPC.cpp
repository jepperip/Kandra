// Fill out your copyright notice in the Description page of Project Settings.

#include "Kandra.h"
#include "SmartNPC.h"
#include <limits>


// Sets default values
ASmartNPC::ASmartNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmartNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmartNPC::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASmartNPC::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ASmartNPC::EvaluateBroadcasts(TArray<FSmartBroadcast> contenders, struct FSmartBroadcast& winner)
{
	float localMin = std::numeric_limits<float>::max();
	for (FSmartBroadcast sb : contenders)
	{
		if (sb.Cost < localMin && sb.Activity == MyCurrentNeed.Activity)
		{
			winner = sb;
			localMin = sb.Cost;
		}
	}

}

void ASmartNPC::CalculateCurrentNeed(struct FEnumWrapper& currentNeed)
{

}

