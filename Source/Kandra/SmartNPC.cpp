// Fill out your copyright notice in the Description page of Project Settings.

#include "Kandra.h"
#include "SmartNPC.h"
#include <limits>

class ASmartObject;

// Sets default values
ASmartNPC::ASmartNPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MyCurrentNeed.Activity = EActivitiesEnum::VE_FIRST;

	MyNeeds = TArray<FNPCNeed>();

	FNPCNeed sleep = FNPCNeed(EActivitiesEnum::VE_Sleep, 0.3f, 0.00034725f, 0.15f);
	FNPCNeed hunger = FNPCNeed(EActivitiesEnum::VE_Eat, 1.0f, 0.0020835f, 0.2f);
	FNPCNeed money = FNPCNeed(EActivitiesEnum::VE_Work, 1.0f, 0.0f, 0.1f);
	FNPCNeed leisure = FNPCNeed(EActivitiesEnum::VE_Leisure, 1.0f, 0.0f, 0.1f);
	MyNeeds.Add(sleep);
	MyNeeds.Add(hunger);
	MyNeeds.Add(money);
	MyNeeds.Add(leisure);

	/*for (uint8 activity = (uint8)EActivitiesEnum::VE_FIRST + 1; activity != (uint8)EActivitiesEnum::VE_LAST; activity++)
	{
		FNPCNeed need;
		need.Activity = (EActivitiesEnum)activity;
		MyNeeds.Add(need);
	}*/

	ObjectMap = TMap<AActor*, struct FSmartBroadcast>();
	
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

//float ASmartNPC::EvaluateBroadcasts(const TArray<FSmartBroadcast>& contenders, struct FSmartBroadcast& winner)
//{
//	float localMax = 0;
//
//	for (FSmartBroadcast broadcast : contenders)
//	{
//		if (broadcast.Range > broadcast.Distance) continue;
//
//		for (FNPCNeed satNeed : broadcast.SatisfyingNeeds)
//		{
//			if (satNeed == MyCurrentNeed)
//			{
//				//Fler grejer h�r
//				float positiveBonus = 0;
//				float negativeBonus = 0;
//
//				//F�r att ge premie till "dyra" st�llen
//				for (FNPCNeed npcNeed : MyNeeds)
//				{
//					if (npcNeed.CurrentValue <= 0) 
//					{
//						npcNeed.CurrentValue = 0.00001f;
//					}
//					for (FNPCNeed penaltyNeed : broadcast.ConsumingNeeds)
//					{
//						if (npcNeed == penaltyNeed)
//						{
//							//(NegativeChangerate^1-Weight) / CurrentValue
//							negativeBonus += (pow(penaltyNeed.ChangeRate, 1 - npcNeed.Weight)) / npcNeed.CurrentValue;
//						}
//					}
//
//					for (FNPCNeed incNeed : broadcast.SatisfyingNeeds)
//					{
//						if (npcNeed == incNeed)
//						{
//							//(PositiveChangerate^1-Weight) / CurrentValue
//							positiveBonus += (pow(incNeed.ChangeRate, 1 - npcNeed.Weight)) / npcNeed.CurrentValue;
//						}
//					}
//				}
//				
//				float activityScore = (1 + (positiveBonus - negativeBonus)) * (1 - broadcast.Cost);
//
//				if (activityScore >= localMax)
//				{
//					winner = broadcast;
//					localMax = activityScore;
//				}
//
//				break;
//			}
//		}
//		
//	}
//	CurrentGoalScore = localMax;
//	return localMax;
//}

float ASmartNPC::EvaluateBroadcasts(struct FSmartBroadcast& winner)
{
	float max = 0;
	auto it = ObjectMap.CreateConstIterator();

	//F�r varje broadcast
	for (it; it; ++it)
	{
		FSmartBroadcast broadcast = it->Value;
		//Om broadcast �r i range
		if (broadcast.Distance > broadcast.Range) continue;
		float totalPositive = 0, totalNegative = 0;

		//R�kna ut kostnad
		double cost = broadcast.Distance / broadcast.Range;

		//F�r varje behov
		for (FNPCNeed npcNeed : MyNeeds){
			float positiveBonus = 0;
			float negativeBonus = 0;

			if (npcNeed.CurrentValue <= 0){
				npcNeed.CurrentValue = 0.00001f;
			}

			//Om broadcasten kostar behovet, l�gg till till broadcast totala negativa v�rde
			for (FNPCNeed penaltyNeed : broadcast.ConsumingNeeds){
				if (npcNeed.Activity == penaltyNeed.Activity){
					totalNegative += (penaltyNeed.ChangeRate * npcNeed.Weight) / npcNeed.CurrentValue;
				}
			}

			//Om broadcasten ger behovet, l�gg till till broadcast totala positiva v�rde
			for (FNPCNeed incNeed : broadcast.SatisfyingNeeds){
				if (npcNeed.Activity == incNeed.Activity){
					totalPositive += (incNeed.ChangeRate * npcNeed.Weight) / npcNeed.CurrentValue;
				}
			}		
		}

		//Broadcast totalpo�ng
		float broadcastScore = (totalPositive - totalNegative) + 1 - cost;

		if (LogBroadcasts){
			USmartTerrainFunctions::LogBroadcast(broadcast, this, broadcastScore, totalPositive, totalNegative);
		}

		//om po�ngen �r b�tter �n current, s�tt broadcasten till tillf�llig vinanre
		if (broadcastScore >= max){
			winner = broadcast;
			max = broadcastScore;
		}
	}

	CurrentGoalScore = max;
	return max;
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

bool ASmartNPC::AddBroadcast_Implementation(const FSmartBroadcast& b)
{
	FSmartBroadcast* it = ObjectMap.Find(b.Sender);
	if (it == NULL)
	{
		//Object does not exist, add a new Object
		ObjectMap.Add(b.Sender, b);
	}
	else
	{
		//Object exists, update it's values
		it->Cost = b.Cost;
		it->Distance = b.Distance;
	}
	
	return true;
}

void ASmartNPC::GetBroadcast(AActor* sender, FSmartBroadcast& Result)
{
	FSmartBroadcast* it = ObjectMap.Find(sender);
	if (it != NULL)
	{
		Result = *it;
	}

}

TArray<FSmartBroadcast> ASmartNPC::GetAllBroadcasts()
{
	TArray<FSmartBroadcast> result = TArray<FSmartBroadcast>();
	
	for (auto it = ObjectMap.CreateConstIterator(); it; ++it)
	{
		result.Add(it->Value);
	}

	return result;
}

bool ASmartNPC::UpdateNeeds_Implementation()
{	
	for (auto it = MyNeeds.CreateIterator(); it; ++it)
	{
		it->CurrentValue -= it->ChangeRate;
	}

	return true;
}


