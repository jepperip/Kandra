// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Character.h"
#include "Kandra/SmartTerrainFunctions.h"
#include "SmartNPC.generated.h"

USTRUCT(BlueprintType)
struct FNPCBrain
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCNeeds")
	float Hunger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCNeeds")
	float Tiredness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCNeeds")
	float WorkNeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCNeeds")
	float OtherNeed;
};

UCLASS()
class KANDRA_API ASmartNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASmartNPC();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
	FNPCBrain MyBrain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	FEnumWrapper MyCurrentNeed;

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
	virtual void EvaluateBroadcasts(TArray<FSmartBroadcast> contenders, struct FSmartBroadcast& winner);

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
	virtual void CalculateCurrentNeed(struct FEnumWrapper& currentNeed);

	
	
};