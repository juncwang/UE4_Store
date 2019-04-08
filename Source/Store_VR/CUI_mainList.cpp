// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_mainList.h"
#include "CameraPawn.h"
#include "Store_VRGameModeBase.h"
#include "BaseMode.h"
#include "NetworkInterface.h"
#include "rapidjson/document.h"

using namespace rapidjson;

#define print(Format, ...) { UE_LOG(LogClass, Warning, Format, ##__VA_ARGS__); }
void UCUI_mainList::initMainList()
{
	if (GetWorld())
	{
		cameraPawn = Cast<ACameraPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		gameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
		gidArray.Empty();
		listDataArray.Empty();
		CPType.Empty();
		cameraPawn->LoadAllBaseActor();

		for (ABaseMode* baseMode : cameraPawn->ModeActorArr)
		{
			bool isGid = true;
			FListData modeData;
			modeData.gid = FString::FromInt(baseMode->gid);
			modeData.number = 1;
			for (int32 i = 0; i<listDataArray.Num(); i++)
			{
				if (listDataArray[i].gid.Equals(modeData.gid))
				{
					listDataArray[i].number += 1.0f;
					isGid = false;
					break;
				}
			}
			if (isGid)
			{
				listDataArray.Push(modeData);
				gidArray.Push(FString::FromInt(baseMode->gid));
			}
		}

		for (AActor* wallActor : cameraPawn->WallActorArr)
		{
			FString left1;
			FString right1;
			FString test1 = wallActor->Tags[wallActor->Tags.Num() - 1].ToString();
			test1.Split("_", &left1, &right1);

			FString left2;
			FString right2;
			FString test2 = wallActor->Tags[1].ToString();
			test2.Split("_", &left2, &right2);

			if (!left1.Equals("W") && left2.Equals("W"))
			{
				gidArray.Push(test1);

				FListData modeData;
				modeData.gid = test1;
				test2.Split("_", &left2, &right2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				modeData.number = FCString::Atof(*right2);

				// ===================================================================================

				FString end = left2;

				FString left3;
				FString right3;

				end.Split("_", &left3, &right3);

				// ===================================================================================

				modeData.productTypeName = right3 + "_";

				listDataArray.Push(modeData);
			}
		}

		for (AActor* flootActor : cameraPawn->FloorActorArr)
		{
			FString left1;
			FString right1;
			FString test1 = flootActor->Tags[flootActor->Tags.Num() - 1].ToString();
			test1.Split("_", &left1, &right1);

			FString left2;
			FString right2;
			FString test2 = flootActor->Tags[1].ToString();
			test2.Split("_", &left2, &right2);

			if (!left1.Equals("F") && left2.Equals("F"))
			{
				gidArray.Push(test1);

				FListData modeData;
				modeData.gid = test1;
				test2.Split("_", &left2, &right2, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
				modeData.number = FCString::Atof(*right2);
				

				// ===================================================================================

				FString end = left2;

				FString left3;
				FString right3;

				end.Split("_", &left3, &right3);

				// ===================================================================================

				modeData.productTypeName = right3 + "_";

				listDataArray.Push(modeData);
			}
		}

		
		if (gidArray.Num() > 0)
		{
			loadPRICESchemeDetail();
		}
		else
		{
			log("initMainList() gidArray is NULL");
		}
		
	}
}

void UCUI_mainList::loadPRICESchemeDetail()
{
	FString gidString = "";
	for (FString test : gidArray)
	{
		if (gidString.Equals(""))
		{
			gidString += test;
		}
		else {
			gidString = gidString + "," + test;
		}
	}

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += PRICE_LIST_INTERFACE + gidString;
	HttpRequest->SetURL(url);

	print(TEXT("loadPRICESchemeDetail : %s"), *url);

// 	for (FListData modeData : listDataArray)
// 	{
// 		FString test = "gid : " + modeData.gid;
// 		test += "     number : " + FString::SanitizeFloat(modeData.number);
// 		test += "     Type : " + modeData.productTypeName;
// 		log(test);
// 	}

	HttpRequest->SetVerb(TEXT("GET"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainList::LoadPRICESchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_mainList::LoadPRICESchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		log(MessageBody);
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		//成功获取网络返回的数据
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			int32 len = int32(jsonDoc["data"].GetArray().Size());
			TotalPrice = 0;
			for (int32 i = 0; i<len; i++)
			{
						listDataArray[i].productName = jsonDoc["data"][i]["productName"].GetFString();
						listDataArray[i].productTypeName += jsonDoc["data"][i]["productTypeName"].GetFString();
						listDataArray[i].productCode = jsonDoc["data"][i]["productCode"].GetFString();
						listDataArray[i].unitPrice = jsonDoc["data"][i]["unitPrice"].GetFloat();
						listDataArray[i].productMarkers = jsonDoc["data"][i]["productMarkers"].GetFString();

						if (jsonDoc["data"][i]["modelGid"].GetInt() == 0)
						{
							if (jsonDoc["data"][i]["materialGid"].GetInt() == 0)
							{
								// 其他数据类型
								// 以后可能会继续添加
							}
							else
							{
								CPType.Push(1);
							}
							
						}
						else {
							CPType.Push(0);
						}


						TotalPrice += jsonDoc["data"][i]["unitPrice"].GetFloat() * listDataArray[i].number;
			}
		}
		initBPMainList();
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		log(MessageBody);
		gameMode->CreateNetErrorUI();
	}
}

void UCUI_mainList::log(FString myString)
{
	if (GEngine)
	{
		myString = "UCUI_mainList::" + myString;
		GEngine->AddOnScreenDebugMessage(-1, 999.0, FColor::Green, myString);
	}
}
