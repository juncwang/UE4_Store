// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_login.h"
#include "Store_VRGameModeBase.h"
#include "CameraGameInstance.h"
#include "NetworkInterface.h"
#include "rapidjson/document.h"
using namespace rapidjson;


void UCUI_login::LoginCallable(FString user, FString pass)
{
	if (GetWorld())
	{
		gameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
		gameIns = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
	}

	FString result;
	FString file = "RuntimeData/BoomAdmin";
	FFileHelper::LoadFileToString(result, *file);
	Document jsonDoc;
	jsonDoc.Parse(result);

	int32 num = int32(jsonDoc["adminUsers"].GetArray().Size());

	bool isAdmin = false;

	for (int32 i = 0; i < num; i++) {
		if (user.Equals(jsonDoc["adminUsers"][i]["username"].GetFString()) && pass.Equals(jsonDoc["adminUsers"][i]["password"].GetFString()))
		{
			isAdmin = true;
		}
	}

	username = user;
	password = pass;
	gameIns->isAdmin = isAdmin;
	if (isAdmin)
	{
		LoginSuccess();
	}
	else {
		userLoginDetail();
	}
}

void UCUI_login::userLoginDetail()
{
	log("void UCUI_login::userLoginDetail()");
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += USER_LOGIN_INTERFACE + username;
	url += "&password=" + password;

	log(url);

	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));

	//UE4�����������ʹ�� BindObject �������а�
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_login::LuserLoginDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_login::LuserLoginDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	log("void UCUI_login::userLoginDetail()");
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		//�ɹ���ȡ���緵�ص�����
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			// ��¼�ɹ�
			LoginSuccess();
		}
		else {
			// ��¼ʧ��
			LoginFailure();
		}

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
}

void UCUI_login::log(FString myString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 99, FColor::Red, myString);
	}
}
