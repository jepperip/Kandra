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

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
	void AddInhabitant(ASmartNPC* npc);

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
	void RemoveInhabitant(ASmartNPC* npc);

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
	void Update();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	TArray<FNPCNeed> SatisfyingNeeds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	TArray<FNPCNeed> ConsumingNeeds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	TArray<ASmartNPC*> Inhabitants;

};
