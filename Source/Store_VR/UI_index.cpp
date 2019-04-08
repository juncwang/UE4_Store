// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_index.h"
#include "rapidjson/document.h"
#include "CameraPlayerController.h"
#include "NetworkInterface.h"
#include "Store_VRGameModeBase.h"
#include "CameraGameInstance.h"

using namespace rapidjson;

void UUI_index::initIndex()
{
	if (GetWorld())
	{
		CameraPlayerController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
		gameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
		cgi = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
	}

	backgroundImage = CameraPlayerController->GetTexture("Image/index_background.png");

	if (backgroundImage)
	{
		UpdataBGImage();
	}

	checkVersion();
}

void UUI_index::checkVersion()
{
	loadVersionSchemeDetail();
}

void UUI_index::ComparedVersion(FString NetVersion)
{
	FString result;
	FString file = "RuntimeData/Version.json";
	FFileHelper::LoadFileToString(result, *file);
	Document jsonDoc;
	jsonDoc.Parse(result);

	localVersion = jsonDoc["Version"].GetFString();

	if (!localVersion.Equals(NetVersion))
	{
		UpdataVersion();
	}
}

void UUI_index::loadVersionSchemeDetail()
{
	log("void UUI_index::loadVersionSchemeDetail()");
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += VERSION_INTERFACE;
	HttpRequest->SetURL(url);
	log(url);
	HttpRequest->SetVerb(TEXT("GET"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UUI_index::LoadVersionSchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UUI_index::LoadVersionSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	log("void UUI_index::loadVersionSchemeDetail()");
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
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
			netVersion = jsonDoc["data"]["clientVersion"].GetFString();
			downloadLink = jsonDoc["data"]["fileUrl"].GetFString();
			ComparedVersion(netVersion);
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}

}

void UUI_index::log(FString myString)
{
	if (GEngine)
	{
		myString = "UUI_index : " + myString;
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, myString);
	}
}
