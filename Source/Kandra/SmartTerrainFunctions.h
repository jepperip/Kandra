// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SmartTerrainFunctions.generated.h"

UENUM(BlueprintType)
enum class EActivitiesEnum : uint8
{
	VE_Sleep UMETA(DisplayName = "Sleep"),
	VE_Eat UMETA(DisplayName = "Eat"),
	VE_Other UMETA(DisplayName = "Other"),
	VE_Work UMETA(DispayName = "Work")
};

USTRUCT()
struct FEnumWrapper
{	
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WrappedEnum")
		EActivitiesEnum Activity;

};

USTRUCT()
struct FSmartBroadcast
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
		AActor* Sender;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
		float Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
		float Distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BroadcastProperties")
		EActivitiesEnum Activity;

};

/**
 * 
 */
UCLASS()
class KANDRA_API USmartTerrainFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
		UFUNCTION(BlueprintCallable, Category = "SmartTerrain")
		static TArray<AActor*> GetActorsInRange(class AActor* self);

		UFUNCTION(BlueprintCallable, Category = "SmartTerrain")
		static void TEST(class AActor* self);

		UFUNCTION(BlueprintCallable, Category = "SmartTerrain")
			static UINT32 VoidTest();


	
};
