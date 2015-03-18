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

