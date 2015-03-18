// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Kandra/SmartNPC.h"
#include "Kandra/SmartTerrainFunctions.h"
#include "SmartObject.generated.h"

//UENUM(BlueprintType)
//enum class EActivitiesEnum : uint8
//{
//	VE_Sleep UMETA(DisplayName = "Sleep"),
//	VE_Eat UMETA(DisplayName = "Eat"),
//	VE_Other UMETA(DisplayName = "Other"),
//	VE_Work UMETA(DispayName = "Work")
//};
//
//USTRUCT(BlueprintType)
//struct FSmartBroadcast
//{
//	GENERATED_USTRUCT_BODY()
//		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
//		AActor* Sender;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
//		float Cost;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
//		float Distance;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
//		EActivitiesEnum Activity;
//
//};




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
	TArray<AActor*> GetActorsInRange();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SmartProperty")
		float Range;

	
	
};
