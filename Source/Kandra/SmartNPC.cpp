// Fill out your copyright notice in the Description page of Project Settings.

#include "Kandra.h"
#include "SmartNPC.h"
#include <limits>

// Sets default values
ASmartNPC::ASmartNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyCurrentNeed.Activity = EActivitiesEnum::VE_FIRST;

	MyNeeds = TArray<FNPCNeed>();
	for (uint8 activity = (uint8)EActivitiesEnum::VE_FIRST + 1; activity != (uint8)EActivitiesEnum::VE_LAST; activity++)
	{
		FNPCNeed need;
		need.Activity = (EActivitiesEnum)activity;
		MyNeeds.Add(need);
	}
	
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

float ASmartNPC::EvaluateBroadcasts(TArray<FSmartBroadcast> contenders, struct FSmartBroadcast& winner)
{
	float localMax = 0;

	for (FSmartBroadcast broadcast : contenders)
	{
		for (FNPCNeed satNeed : broadcast.SatisfyingNeeds)
		{
			if (satNeed.Activity == MyCurrentNeed.Activity)
			{
				//Fler grejer här
				float positiveBonus = 0;
				float negativeBonus = 0;

				//För att ge premie till "dyra" ställen
				for (FNPCNeed npcNeed : MyNeeds)
				{
					if (npcNeed.CurrentValue <= 0) 
					{
						npcNeed.CurrentValue = 0.00001f;
					}
					for (FNPCNeed penaltyNeed : broadcast.ConsumingNeeds)
					{
						if (npcNeed.Activity == penaltyNeed.Activity)
						{
							//(NegativeChangerate^1-Weight) / CurrentValue
							negativeBonus += (pow(penaltyNeed.ChangeRate, 1 - npcNeed.Weight)) / npcNeed.CurrentValue;
						}
					}

					for (FNPCNeed incNeed : broadcast.SatisfyingNeeds)
					{
						if (npcNeed.Activity == incNeed.Activity)
						{
							//(PositiveChangerate^1-Weight) / CurrentValue
							positiveBonus += (pow(incNeed.ChangeRate, 1 - npcNeed.Weight)) / npcNeed.CurrentValue;
						}
					}
				}
				
				float activityScore = (1 + (positiveBonus - negativeBonus)) / broadcast.Cost;

				if (activityScore >= localMax)
				{
					winner = broadcast;
					localMax = activityScore;
				}

				break;
			}
		}
		
	}
	CurrentGoalScore = localMax;
	return localMax;
}

float ASmartNPC::CalculateCurrentNeed()
{
	float localMin = std::numeric_limits<float>::max();
	
	FNPCNeed* temp = NULL;
	for (int32 i = 0; i < MyNeeds.Num(); i++)
	{
		temp = &MyNeeds[i];
		if (!temp)continue;
		float needScore = temp->CurrentValue * (1 - temp->Weight);
		//needScore = pow(temp->CurrentValue, 1 - temp->Weight);
		if (needScore < localMin)
		{
			localMin = needScore;
			MyCurrentNeed = *temp;
		}

	}

	return localMin;
}

bool ASmartNPC::AddBroadcast_Implementation(FSmartBroadcast b)
{
	return true;
}

