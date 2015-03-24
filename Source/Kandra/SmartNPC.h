// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Character.h"
#include "Kandra/SmartTerrainFunctions.h"
#include "SmartNPC.generated.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	TArray<FNPCNeed> MyNeeds;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SmartProperty")
	float CurrentGoalScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	FNPCNeed MyCurrentNeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SmartProperty")
	TArray<FSmartBroadcast> myBroadcasts;

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
		virtual float EvaluateBroadcasts(TArray<FSmartBroadcast> contenders, struct FSmartBroadcast& winner);

	UFUNCTION(BlueprintCallable, Category = "SmartFunctions")
		virtual float CalculateCurrentNeed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SmartFunctions")
	bool AddBroadcast(FSmartBroadcast b);
	virtual bool AddBroadcast_Implementation(FSmartBroadcast b);

	
	
};
