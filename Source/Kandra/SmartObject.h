// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Kandra/SmartNPC.h"
#include "Kandra/SmartTerrainFunctions.h"
#include "SmartObject.generated.h"

UCLASS()
class KANDRA_API ASmartObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmartObject();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
		void Broadcast(FSmartBroadcast b, ASmartNPC* aNpc);

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
	TArray<AActor*> GetActorsInRange();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	EActivitiesEnum Activity;

	
	
};
