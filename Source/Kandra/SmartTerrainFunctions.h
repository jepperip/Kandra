// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SmartTerrainFunctions.generated.h"

UENUM(BlueprintType)
enum class EActivitiesEnum : uint8
{
	VE_FIRST UMETA(DisplayName = "Must come first"),

	VE_Sleep UMETA(DisplayName = "Sleep"),
	VE_Eat UMETA(DisplayName = "Eat"),
	VE_Other UMETA(DisplayName = "Other"),
	VE_Work UMETA(DispayName = "Work"),

	VE_LAST UMETA(DisplayName = "Must come last")
};

USTRUCT()
struct FEnumWrapper
{	
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WrappedEnum")
	EActivitiesEnum Activity;

};

USTRUCT(BlueprintType)
struct FNPCNeed
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NeedProperties")
	EActivitiesEnum Activity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NeedProperties")
	float CurrentValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NeedProperties")
	float ChangeRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NeedProperties")
	float Weight;

	bool operator==(const FNPCNeed& other)
	{
		return other.Activity == this->Activity;
	}

	FNPCNeed()
	{
		CurrentValue = 0.5f;
		ChangeRate = 0.01f;
		Weight = 0.2f;
		Activity = EActivitiesEnum::VE_FIRST;
	}

};

USTRUCT(BlueprintType)
struct FSmartBroadcast
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
	AActor* Sender;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
	float Cost;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
	float Distance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
	float Range;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
	TArray<FNPCNeed> SatisfyingNeeds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
	TArray<FNPCNeed> ConsumingNeeds;

	bool operator== (const FSmartBroadcast& other)
	{
		return other.Sender == this->Sender;
	}

	bool SatisfiesNeed(const FNPCNeed& need) const
	{
		for (FNPCNeed n : SatisfyingNeeds)
		{
			if (n == need)
			{
				return true;
			}
		}
		return false;
	}
};

/**
 * 
 */
UCLASS()
class KANDRA_API USmartTerrainFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

};
