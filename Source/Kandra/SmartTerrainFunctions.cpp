// Fill out your copyright notice in the Description page of Project Settings.

#include "Kandra.h"
#include "SmartTerrainFunctions.h"
#include "Kandra/SmartNPC.h"
#include "Kandra/SmartObject.h"

const string USmartTerrainFunctions::RootDirectory = "LOGGING";
const string USmartTerrainFunctions::versionFormat = "v";
const string USmartTerrainFunctions::fileEnding = ".log";
string USmartTerrainFunctions::SessionDirectory = string();
bool USmartTerrainFunctions::newSession = true;
bool USmartTerrainFunctions::sessionInitialized = false;
int USmartTerrainFunctions::versionNumber = 0;

void USmartTerrainFunctions::CreateLoggingFile(string fileName, string labels)
{
	time_t now = time(0);
	char* date = ctime(&now);
	ofstream stream;
	stream.open(fileName + fileEnding, ios_base::app);
	stream << date << '\n';
	stream << labels << '\n';
	stream.close();
}

bool USmartTerrainFunctions::StartNewSession(TArray<FString> needLabels)
{
	time_t now = time(0);
	char* date = ctime(&now);
	string dateFormatted = date;
	dateFormatted.erase(remove_if(dateFormatted.begin(), dateFormatted.end(), isspace), dateFormatted.end());
	dateFormatted.erase(remove(dateFormatted.begin(), dateFormatted.end(), ':'), dateFormatted.end());

	FString p = FPaths::GameDir();
	p += RootDirectory.c_str();
	string path = string(TCHAR_TO_UTF8(*p));
	path += "/" + dateFormatted;
	FString pathTest = path.c_str();

	if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*pathTest)){
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "LOGGING: Path: '" + pathTest + "' already exists. Logging session NOT started");
		return false;
	}
	FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*pathTest);
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*pathTest)){
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "LOGGING: Could not create path: '" + pathTest + "'. Logging session NOT started");
		return false;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "LOGGING: Created path: '" + pathTest + "', logging session started");



	//Create files
	string labForm = string("Tid\t");
	for (auto it = needLabels.CreateConstIterator(); it; ++it)
	{
		FString f = *it;
		labForm += TCHAR_TO_UTF8(*f);
		labForm += '\t';
	}
	
	CreateLoggingFile(path + "/Schedule", "Time\tNeed\t");
	CreateLoggingFile(path + "/Needs", labForm);
	CreateLoggingFile(path + "/Broadcasts", "NPC\tObject\tScore\tPositiv\tnNegativ\nAvstnd");

	sessionInitialized = true;
	SessionDirectory = path;
	return true;
}

void USmartTerrainFunctions::SaveLog(string fileName, string data)
{
	ofstream stream;
	stream.open(SessionDirectory + '/' + fileName + fileEnding, ios_base::app);
	stream << data << '\n';
	stream.close();
}

bool USmartTerrainFunctions::SaveToFile_SaveStringTextToFile(FString fileName, FString SaveText, FString& Result){
	FString path;
	path = FPaths::GameDir();
	path += RootDirectory.c_str();
	if (!sessionInitialized)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "LOGGING: No Session Started before logging. No data will be logged");
		Result = "FAIL";
		return false;
	}

	if (newSession)
	{
		time_t now = time(0);
		char* date = ctime(&now);
		path += "/";
		path += date;
	}

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
	stream.open(TCHAR_TO_UTF8(*temp), ios_base::app);
	if (newSession)
	{
		newSession = false;
		time_t now = time(0);
		char* dt = ctime(&now);
		stream << dt << '\n';
	}
	string data(TCHAR_TO_UTF8(*SaveText));
	stream << data << '\n';
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

bool USmartTerrainFunctions::SaveNPCNeeds(AActor* SmartNpc, const int32 h, const int32 m, const int32 s, FString& Result)
{
	ASmartNPC* npc = Cast<ASmartNPC>(SmartNpc);
	if (!npc)
	{
		Result = "Invalid parameter: SmartNpc";
		return false;
	}
	
	FString name = npc->GetName();
	name += " Needs";

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

	FString data = time.c_str();
	SaveLog("Needs", time);
	//SaveToFile_SaveStringTextToFile(name, data, Result);

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

	string data = string();
	data += TCHAR_TO_UTF8(*npc->GetName());
	data += "\t";
	data += TCHAR_TO_UTF8(*sender->GetName());
	data += "\t";
	data += to_string(score);
	data += "\t";
	data += to_string(positive);
	data += "\t";
	data += to_string(negative);
	data += "\t";
	data += to_string(b.Distance);

	SaveLog("Broadcasts", data);
	FString res = FString();
	//SaveToFile_SaveStringTextToFile(name, data.c_str(), res);

	return true;

}

bool USmartTerrainFunctions::LogSchedule(AActor* so, AActor* npc, TArray<FString> needs, const int32 h, const int32 m)
{
	ASmartNPC* n = Cast<ASmartNPC>(npc);
	if (!n)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Invalid parameter: npc");
		return false;
	}

	ASmartObject* o = Cast<ASmartObject>(so);
	if (!o)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Invalid parameter: so");
		return false;
	}

	string data = string();
	if (h < 10) data += '0';
	data += to_string(h) + ":";
	if (m < 10) data += '0';
	data += to_string(m) + '\t';

	for (FString f : needs)
	{
		data += TCHAR_TO_UTF8(*f);
		data += '\t';
	}
	
	SaveLog("Schedule", data);

	return true;

}
