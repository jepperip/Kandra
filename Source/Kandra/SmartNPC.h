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
		TArray<FNPCNeed> Needs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPCNeeds")
		FNPCNeed mostUrgentNeed;

	void Initialize()
	{
		Needs = TArray<FNPCNeed>();
		for (uint8 activity = (uint8)EActivitiesEnum::VE_FIRST + 1; activity != (uint8)EActivitiesEnum::VE_LAST; activity++)
		{
			FNPCNeed need;
			need.Activity = (EActivitiesEnum)activity;
			Needs.Add(need);
		}

	}

	void Update();
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	FNPCBrain MyBrain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	FEnumWrapper MyCurrentNeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	TArray<FSmartBroadcast> myBroadcasts;

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
	virtual void EvaluateBroadcasts(TArray<FSmartBroadcast> contenders, struct FSmartBroadcast& winner);

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
	virtual bool CalculateCurrentNeed(struct FEnumWrapper& currentNeed);

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
		virtual bool UpdateBrain();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SmartFunctions")
		bool AddBroadcast(FSmartBroadcast b);
	virtual bool AddBroadcast_Implementation(FSmartBroadcast b);

	
	
};
