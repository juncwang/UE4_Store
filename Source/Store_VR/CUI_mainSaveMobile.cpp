// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_mainSaveMobile.h"
#include "CameraGameInstance.h"
#include "Store_VRGameModeBase.h"
#include "CameraPawn.h"
#include "rapidjson/document.h"
#include "public/HAL/PlatformFilemanager.h"
#include "BaseMode.h"
#include "NetworkInterface.h"

using namespace rapidjson;

#define print(Format, ...) { UE_LOG(LogClass, Warning, Format, ##__VA_ARGS__); }
void UCUI_mainSaveMobile::SetJsonActorAll(FString phone)
{
	ACameraPawn* cameraPawn = nullptr;
	UCameraGameInstance* cameraGI = nullptr;
	JsonActorAll = "{";
	if (GetWorld())
	{
		cameraPawn = Cast<ACameraPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		cameraGI = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		gamemode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());

		roomTypeInfoGid = cameraGI->gid;
		gid = cameraGI->gidScheme;
		userPhone = phone;

		cameraPawn->LoadAllBaseActor();

		JsonActorAll += "\"model\":[";
		for (int32 i = 0; i < cameraPawn->ModeActorArr.Num(); i++)
		{
			ABaseMode* actor = cameraPawn->ModeActorArr[i];
			if (actor->gid > 0)
			{
				JsonActorAll += "{\"gid\":\"";
				JsonActorAll += FString::FromInt(actor->gid);
				JsonActorAll += "\",\"transform\":{\"location\":{\"x\":";
				JsonActorAll += FString::SanitizeFloat(actor->GetActorLocation().X);
				JsonActorAll += ",\"y\":";
				JsonActorAll += FString::SanitizeFloat(actor->GetActorLocation().Y);
				JsonActorAll += ",\"z\":";
				JsonActorAll += FString::SanitizeFloat(actor->GetActorLocation().Z);
				JsonActorAll += "},\"rotation\":{\"roll\":";
				JsonActorAll += FString::SanitizeFloat(actor->GetActorRotation().Roll);
				JsonActorAll += ",\"pitch\":";
				JsonActorAll += FString::SanitizeFloat(actor->GetActorRotation().Pitch);
				JsonActorAll += ",\"yaw\":";
				JsonActorAll += FString::SanitizeFloat(actor->GetActorRotation().Yaw);
				JsonActorAll += "},\"scale\":{\"x\":";
				JsonActorAll += FString::SanitizeFloat(actor->GetActorScale().X);
				JsonActorAll += ",\"y\":";
				JsonActorAll += FString::SanitizeFloat(actor->GetActorScale().Y);
				JsonActorAll += ",\"z\":";
				JsonActorAll += FString::SanitizeFloat(actor->GetActorScale().Z);
				JsonActorAll += "}}}";
				if (i != (cameraPawn->ModeActorArr.Num()-1))
				{
					JsonActorAll += ",";
				}
			}
		}
		JsonActorAll += "],";

		JsonActorAll += "\"material\":[";

		bool bFloorActor = false;
		bool bActor = false;

		for (int32 i = 0; i < cameraPawn->FloorActorArr.Num(); i++)
		{
			AActor* actor = cameraPawn->FloorActorArr[i];
			if (actor->Tags.Num() > 2)
			{
				if (bActor)
				{
					JsonActorAll += ",";
					bActor = false;
				}
				JsonActorAll += "{\"gid\":\"";
				JsonActorAll += actor->Tags[actor->Tags.Num() - 1].ToString();
				JsonActorAll += "\",\"modelName\":\"";
				JsonActorAll += actor->GetName();
				JsonActorAll += "\"}";
				bActor = true;
				bFloorActor = true;
			}
		}
		bActor = false;
		for (int32 i = 0; i < cameraPawn->WallActorArr.Num(); i++)
		{
			AActor* actor = cameraPawn->WallActorArr[i];
			if (actor->Tags.Num() > 2)
			{
				if (bFloorActor)
				{
					JsonActorAll += ",";
					bFloorActor = false;
				}
				if (bActor)
				{
					JsonActorAll += ",";
					bActor = false;
				}
				JsonActorAll += "{\"gid\":\"";
				JsonActorAll += actor->Tags[actor->Tags.Num() - 1].ToString();
				JsonActorAll += "\",\"modelName\":\"";
				JsonActorAll += actor->GetName();
				JsonActorAll += "\"}";
				bActor = true;
			}
		}

		JsonActorAll += "]";

	}
	JsonActorAll += "}";
	print(TEXT("init_mainImage %s"), *JsonActorAll);
}

void UCUI_mainSaveMobile::startSave()
{
	uploadConfigureFile();
}

void UCUI_mainSaveMobile::uploadConfigureFile()
{

	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(TEXT("project.json"));
	FString str = JsonActorAll;
	FFileHelper::SaveStringToFile(str, TEXT("project.json"), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url.Append(UPLOAD_FILE);

	TArray<uint8> filedata;
	if (!FFileHelper::LoadFileToArray(filedata, TEXT("project.json")))
	{
		return;
	}
	HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=blahblahsomeboundary");

	FString a = "\r\n--blahblahsomeboundary\r\n";
	FString b = "Content-Disposition: form-data; name=\"file\";  filename=\"project.json\"\r\n";

	FString c = "Content-Type: application/octet-stream\r\n\r\n";
	//d = UpFileRawData
	FString e = "\r\n--blahblahsomeboundary--\r\n";


	TArray<uint8> data;
	data.Append((uint8*)TCHAR_TO_UTF8(*a), a.Len());
	data.Append((uint8*)TCHAR_TO_UTF8(*b), b.Len());
	data.Append((uint8*)TCHAR_TO_UTF8(*c), c.Len());
	data.Append(filedata);
	data.Append((uint8*)TCHAR_TO_UTF8(*e), e.Len());

	HttpRequest->SetContent(data);


	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainSaveMobile::uploadConfigureFileCompleteCallback);
	HttpRequest->ProcessRequest();

}


void UCUI_mainSaveMobile::uploadConfigureFileCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogClass, Warning, TEXT("uploadConfigureFileCompleteCallback"));

	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gamemode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			configureFile = SUploadItem();
			configureFile.gid = jsonDoc["data"]["gid"].GetInt();
			configureFile.fileName = jsonDoc["data"]["fileName"].GetFString();
			textFileUrl = jsonDoc["data"]["fileName"].GetFString();
			// 上传完成继续上传图片
			uploadImage();
		}
		else
		{
			gamemode->CreateNetErrorUI();
			// 上传失败
		}

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gamemode->CreateNetErrorUI();
	}
	//UE_LOG(LogClass, Warning, TEXT("%s"), *MessageBody);
	print(TEXT("%s"), *MessageBody);
}

void UCUI_mainSaveMobile::uploadImage()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url.Append(UPLOAD_FILE);

	HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=blahblahsomeboundary");

	FString filePath = gamemode->GetFileDir(UE_BUILD_SHIPPING) + "HighresScreenshot00000.png";

	TArray<uint8> filedata;
	if (!FFileHelper::LoadFileToArray(filedata, *filePath))
	{
		// 
		return;
	}

	FString a = "\r\n--blahblahsomeboundary\r\n";
	FString b = FString::Printf(TEXT("Content-Disposition: form-data; name=\"file\";  filename=\"%s\"\r\n"), *filePath);
	print(TEXT("b:%s"), *b);
	FString c = "Content-Type: application/octet-stream\r\n\r\n";
	//d = UpFileRawData
	FString e = "\r\n--blahblahsomeboundary--\r\n";


	TArray<uint8> data;
	data.Append((uint8*)TCHAR_TO_UTF8(*a), a.Len());
	data.Append((uint8*)TCHAR_TO_UTF8(*b), b.Len());
	data.Append((uint8*)TCHAR_TO_UTF8(*c), c.Len());
	data.Append(filedata);
	data.Append((uint8*)TCHAR_TO_UTF8(*e), e.Len());

	HttpRequest->SetContent(data);


	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainSaveMobile::uploadImageCompleteCallback);
	HttpRequest->ProcessRequest();
}

void UCUI_mainSaveMobile::uploadImageCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	UE_LOG(LogClass, Warning, TEXT("uploadImageCompleteCallback"));

	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gamemode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			configureFile = SUploadItem();
			configureFile.gid = jsonDoc["data"]["gid"].GetInt();
			configureFile.fileName = jsonDoc["data"]["fileName"].GetFString();
			showImgUrl = jsonDoc["data"]["fileName"].GetFString();
			// 上传完成发送完成请求
			requestFinish();
		}
		else
		{
			gamemode->CreateNetErrorUI();
			// 上传失败
			//UE_LOG(LogClass, Warning, TEXT("上传失败"));
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gamemode->CreateNetErrorUI();
	}
	//UE_LOG(LogClass, Warning, TEXT("%s"), *MessageBody);
	print(TEXT("%s"), *MessageBody);
}

void UCUI_mainSaveMobile::requestFinish()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += SAVESCHEME_INTERFACE;
	url += "?roomTypeInfoGid=";
	url += roomTypeInfoGid;
	url += "&userPhone=";
	url += userPhone;
	url += "&textFileUrl=";
	url += textFileUrl;
	url += "&showImgUrl=";
	url += showImgUrl;

	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainSaveMobile::requestFinishCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_mainSaveMobile::requestFinishCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogClass, Warning, TEXT("requestFinishCompleteCallback"));

	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gamemode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			// 保存成功
		}
		else
		{
			gamemode->CreateNetErrorUI();
			// 保存失败
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gamemode->CreateNetErrorUI();
	}
	UE_LOG(LogClass, Warning, TEXT("%s"), *MessageBody);
}
