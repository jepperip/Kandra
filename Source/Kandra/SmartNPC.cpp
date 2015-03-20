// Fill out your copyright notice in the Description page of Project Settings.

#include "Kandra.h"
#include "SmartNPC.h"
#include <limits>

void FNPCBrain::Update()
{

}

// Sets default values
ASmartNPC::ASmartNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyBrain = FNPCBrain();
	MyBrain.Initialize();
	MyCurrentNeed.Activity = EActivitiesEnum::VE_FIRST;
	
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
	float localMin = 0;

	for (FSmartBroadcast sb : contenders)
	{
		for (FNPCNeed sn : sb.SatisfyingNeeds)
		{
			if (sn.Activity == MyCurrentNeed.Activity)
			{
				//Fler grejer här
				float activityScore = sn.ChangeRate / sb.Cost;
				if (activityScore < localMin)
				{
					winner = sb;
					localMin = activityScore;
				}
			}
		}
		
	}
}

bool ASmartNPC::CalculateCurrentNeed(struct FEnumWrapper& currentNeed)
{
	float localMin = std::numeric_limits<float>::max();

	for (FNPCNeed e : MyBrain.Needs)
	{
		float needScore = e.CurrentValue * e.Weight;
		if (needScore < localMin)
		{
			currentNeed.Activity = e.Activity;
		}
	}
	return true;
}

bool ASmartNPC::UpdateBrain()
{
	MyBrain.Update();
	return true;
}

bool ASmartNPC::AddBroadcast_Implementation(FSmartBroadcast b)
{
	return true;
}

