// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SmartTerrainFunctions.generated.h"

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

		static void TEST(class AActor* self);

	
};
