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

float ASmartNPC::EvaluateBroadcasts(struct FSmartBroadcast& winner)
{
	float max = 0;
	auto it = ObjectMap.CreateConstIterator();

	//För varje broadcast
	for (it; it; ++it)
	{
		FSmartBroadcast broadcast = it->Value;
		//Om broadcast är i range
		if (broadcast.Distance > broadcast.Range) continue;
		float totalPositive = 0, totalNegative = 0;

		//Räkna ut kostnad
		double cost = broadcast.Distance / broadcast.Range;
		
		bool notMyHome = false;
		bool notMyWorkplace = false;

		if (Home != NULL){
			for (FNPCNeed incNeed : broadcast.SatisfyingNeeds){
				if (incNeed.Activity == EActivitiesEnum::VE_Sleep){
					notMyHome = (broadcast.Sender != Home);
				}
			}
		}

		if (Workplace != NULL){
			for (FNPCNeed incNeed : broadcast.SatisfyingNeeds){
				if (incNeed.Activity == EActivitiesEnum::VE_Work){
					notMyWorkplace = (broadcast.Sender != Workplace);
				}
			}
		}

		if (notMyHome || notMyWorkplace) continue;

		//För varje behov
		for (FNPCNeed npcNeed : MyNeeds){
			float positiveBonus = 0;
			float negativeBonus = 0;

			//Check for home and workplace
			if (Home != NULL && npcNeed.Activity == EActivitiesEnum::VE_Sleep){
				
			}

			if (Workplace != NULL && npcNeed.Activity == EActivitiesEnum::VE_Work){

			}

			if (npcNeed.CurrentValue <= 0){
				npcNeed.CurrentValue = 0.00001f;
			}

			//Om broadcasten kostar behovet, lägg till till broadcast totala negativa värde
			for (FNPCNeed penaltyNeed : broadcast.ConsumingNeeds){
				if (npcNeed.Activity == penaltyNeed.Activity){
					totalNegative += (penaltyNeed.ChangeRate * npcNeed.Weight) / npcNeed.CurrentValue;
				}
			}

			//Om broadcasten ger behovet, lägg till till broadcast totala positiva värde
			for (FNPCNeed incNeed : broadcast.SatisfyingNeeds){
				if (npcNeed.Activity == incNeed.Activity){
					totalPositive += (incNeed.ChangeRate * npcNeed.Weight) / npcNeed.CurrentValue;
				}
			}		
		}

		//Broadcast totalpoäng
		float broadcastScore = (totalPositive - totalNegative) + 1 - cost;

		if (LogBroadcasts){
			USmartTerrainFunctions::LogBroadcast(broadcast, this, broadcastScore, totalPositive, totalNegative);
		}

		//om poängen är bätter än current, sätt broadcasten till tillfällig vinanre
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


