// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include <string>
#include <fstream>
#include <ctime>
#include <algorithm>
using namespace std;
#include "SmartTerrainFunctions.generated.h"

UENUM(BlueprintType)
enum class EActivitiesEnum : uint8
{
	VE_FIRST UMETA(DisplayName = "Must come first"),

	VE_Sleep UMETA(DisplayName = "Sleep"),
	VE_Eat UMETA(DisplayName = "Eat"),
	VE_Leisure UMETA(DisplayName = "Leisure"),
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

	FNPCNeed(EActivitiesEnum activity, float startValue, float changeRate, float Weight)
	{
		CurrentValue = startValue;
		ChangeRate = changeRate;
		this->Weight = Weight;
		Activity = activity;
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
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "StartNewSession"), Category = "LOGGING")
		static bool StartNewSession(TArray<FString> needLabels);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "SaveToFile"), Category = "LogginToolsForStani")
		static bool SaveToFile_SaveStringTextToFile(FString fileName, FString SaveText, FString& Result);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "SaveActorPosition"), Category = "LOGGING")
		static bool SaveToFile_SaveActorPosition(AActor* actor, FString& Result);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "SaveNPCNeeds"), Category = "LOGGING")
		static bool SaveNPCNeeds(AActor* SmartNpc, const int32 h, const int32 m, const int32 s, FString& Result);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "LogBroadcast"), Category = "LOGGING")
		static bool LogBroadcast(const FSmartBroadcast& b, AActor* npc, float score, float positive, float negative);

	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "LogSchedule"), Category = "LOGGING")
		static bool LogSchedule(AActor* so, AActor* npc, TArray<FString> needs, const int32 h, const int32 m);

private:

	inline static float GetElapsedTime(UWorld* contex) { return contex->GetTimeSeconds(); }
	inline static float GetElapsedTime(AActor* contexHolder) { return contexHolder->GetWorld()->GetTimeSeconds(); }

	static void SaveLog(string fileName, string data);

	static void CreateLoggingFile(string fileName, string labels);

	static const string RootDirectory;
	static const string versionFormat;
	static const string fileEnding;
	static string version;
	static string SessionDirectory;
	static bool newSession;
	static bool sessionInitialized;
	static int versionNumber;

};
