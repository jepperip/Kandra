// Fill out your copyright notice in the Description page of Project Settings.

#include "Kandra.h"
#include "SmartTerrainFunctions.h"
#include "Kandra/SmartNPC.h"

const string USmartTerrainFunctions::outputDir = "LOGGING";
const string USmartTerrainFunctions::versionFormat = "v";
const string USmartTerrainFunctions::fileEnding = ".log";
FString USmartTerrainFunctions::file = "0";
bool USmartTerrainFunctions::newSession = true;
bool USmartTerrainFunctions::firstTimeLoggingNeeds = true;
bool USmartTerrainFunctions::firstTimeLoggingBroadcasts = true;
int USmartTerrainFunctions::versionNumber = 0;

bool USmartTerrainFunctions::SaveToFile_SaveStringTextToFile(FString fileName, FString SaveText, FString& Result){
	FString path;
	path = FPaths::GameDir();
	path += outputDir.c_str();

	//check if directory exists. If not: create it
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*path)){
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*path);

		if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*path)){
			return false;
		}
	}
	//Append filename to the path
	path += "/" + fileName + "." + versionFormat.c_str();

	//Create temporary string to search with
	FString temp = path + to_string(versionNumber).c_str() + fileEnding.c_str();

	//Find version
	if (newSession)
	{
		while (FPlatformFileManager::Get().GetPlatformFile().FileExists(*temp))
		{
			versionNumber++;
			temp = path + to_string(versionNumber).c_str() + fileEnding.c_str();
		}
	}

	//Save data to the file	
	ofstream stream;
	file = temp;
	stream.open(TCHAR_TO_UTF8(*temp), ios_base::app);
	if (newSession)
	{
		newSession = false;
		time_t now = time(0);
		char* dt = ctime(&now);
		stream << dt << '\n';
	}
	string data(TCHAR_TO_UTF8(*SaveText));
	stream << data;
	stream.close();

	Result = path + " : " + SaveText;
	return true;
}

bool USmartTerrainFunctions::SaveToFile_SaveActorPosition(AActor* actor, FString& Result)
{
	FString name = actor->GetName();
	name += " Position";
	FVector pos3 = actor->GetActorLocation();
	FVector2D pos2 = FVector2D(pos3.X, pos3.Y);
	FString x = FString::SanitizeFloat(pos2.X);
	FString y = FString::SanitizeFloat(pos2.Y);
	FString data(x + "\t" + y + "\n");
	SaveToFile_SaveStringTextToFile(name, data, Result);

	return true;
}

bool USmartTerrainFunctions::SaveNPCNeeds(AActor* SmartNpc, TArray<FString> labels, const int32 h, const int32 m, const int32 s, FString& Result)
{
	ASmartNPC* npc = Cast<ASmartNPC>(SmartNpc);
	if (!npc)
	{
		Result = "Invalid parameter: SmartNpc";
		return false;
	}
	if (labels.Num() <= 0)
	{
		Result = "Please provide Labels";
		return false;
	}
	
	FString name = npc->GetName();
	name += " Needs";
	if (firstTimeLoggingNeeds)
	{
		firstTimeLoggingNeeds = false;
		string labForm = string("Tid\t");
		for (auto it = labels.CreateConstIterator(); it; ++it)
		{
			FString f = *it;
			labForm += TCHAR_TO_UTF8(*f);
			labForm += '\t';
		}
		labForm += '\n';
		FString res = FString();
		SaveToFile_SaveStringTextToFile(name, labForm.c_str(), res);
	}

	auto it = npc->MyNeeds.CreateConstIterator();
	string time = string();
	if (h < 10) time += '0';
	time += to_string(h) + ":";
	if (m < 10) time += '0';
	time += to_string(m) + ":";
	if (s < 10) time += '0';
	time += to_string(s) + '\t';

	for (it; it; ++it)
	{
		time += to_string(it->CurrentValue) + '\t';
	}
	time += '\n';

	FString data = time.c_str();
	SaveToFile_SaveStringTextToFile(name, data, Result);

	return true;
}

bool USmartTerrainFunctions::LogBroadcast(const FSmartBroadcast& b, AActor* npc, float score, float positive, float negative)
{
	FString name = npc->GetName();
	AActor* sender = b.Sender;
	if (!sender)
	{
		return false;
	}
	name += " Broadcasts";

	string data = string("\t");
	data += "NPC: ";
	data += TCHAR_TO_UTF8(*npc->GetName());
	data += "\tObject: ";
	data += TCHAR_TO_UTF8(*sender->GetName());
	data += "\tScore: ";
	data += to_string(score);
	data += "\tPossyB: ";
	data += to_string(positive);
	data += "\tNeggoB: ";
	data += to_string(negative);
	data += "\tDistance: ";
	data += to_string(b.Distance);
	data += "\tCost: ";
	data += to_string(b.Cost);
	data += "\n";

	FString res = FString();
	SaveToFile_SaveStringTextToFile(name, data.c_str(), res);

	return true;

}
