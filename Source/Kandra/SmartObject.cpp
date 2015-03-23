// Fill out your copyright notice in the Description page of Project Settings.

#include "Kandra.h"
#include "SmartObject.h"


// Sets default values
ASmartObject::ASmartObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmartObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmartObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASmartObject::Broadcast(FSmartBroadcast b, ASmartNPC* aNpc)
{
	aNpc->AddBroadcast(b);

}

void ASmartObject::Update()
{
	//hur NPC'n påverkas av att spendera tid i huset
	//Superseg algoritm
	for (ASmartNPC* npc : Inhabitants)
	{
		FNPCNeed* npcNeed = NULL;
		for (int32 i = 0; i < npc->MyNeeds.Num(); i++)
		{
			npcNeed = &npc->MyNeeds[i];
			if (!npcNeed)continue;
			for (FNPCNeed satNeed : SatisfyingNeeds)
			{
				//Tredjegrasfunktion
				if (satNeed.Activity == npcNeed->Activity)
				{
					npcNeed->CurrentValue += satNeed.ChangeRate;
					break;
				}
			}

			for (FNPCNeed consNeed : ConsumingNeeds)
			{
				if (consNeed.Activity == npcNeed->Activity)
				{
					npcNeed->CurrentValue -= consNeed.ChangeRate;
					break;
				}
			}
		}
	}
}

TArray<AActor*> ASmartObject::GetActorsInRange()
{
	TArray<AActor*> res = TArray<AActor*>();
	FVector pos = GetActorLocation();
	
	for (TActorIterator<ACharacter> allActors(GetWorld()); allActors; ++allActors)
	{
		FVector NPCPos = allActors->GetActorLocation();


			if (FVector::Dist(NPCPos, pos) < Range) res.Push(*allActors);
		
	}
	
	return res;
}

void ASmartObject::AddInhabitant(ASmartNPC* npc)
{
	Inhabitants.Add(npc);
}

void ASmartObject::RemoveInhabitant(ASmartNPC* npc)
{
	Inhabitants.Remove(npc);
}

