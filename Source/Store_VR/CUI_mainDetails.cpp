// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_mainDetails.h"
#include "CameraPawn.h"
#include "Store_VRGameModeBase.h"
#include "CameraPlayerController.h"
#include "NetworkInterface.h"
#include "BaseMode.h"
#include "Tools.h"
#include "HAL/PlatformFilemanager.h"
#include "rapidjson/document.h"

using namespace rapidjson;

void UCUI_mainDetails::initDetails()
{
	gid = "";
	if (GetWorld())
	{
		cameraPawn = Cast<ACameraPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		gameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
		cameraController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
		if (cameraPawn->PawnBaseMode != nullptr || cameraPawn->WorF_Actor !=nullptr)
		{
			if (cameraPawn->PawnBaseMode != nullptr)
			{
				gid = FString::FromInt(cameraPawn->PawnBaseMode->gid);
			}
			else {
				gid = cameraPawn->WorF_Actor->Tags[cameraPawn->WorF_Actor->Tags.Num() - 1].ToString();
			}
			if (!gid.Equals(""))
			{
				loadPRODUCTSchemeDetail();
			}
			else
			{
				EventClose();
			}
		}
	}
}

void UCUI_mainDetails::loadPRODUCTSchemeDetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += PRODUCT_INTERFACE + gid;
	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainDetails::LoadPRODUCTSchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_mainDetails::LoadPRODUCTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
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

		UnitPrice = FString::SanitizeFloat(jsonDoc["data"]["productUnitPrice"].GetFloat());
		BrandAndProductName = jsonDoc["data"]["brandInfoName"].GetFString() + " / " + jsonDoc["data"]["productName"].GetFString();
		StyleName = jsonDoc["data"]["configStyleName"].GetFString();
		MaterialName = jsonDoc["data"]["productTypeName"].GetFString();
		SizeName = FString::SanitizeFloat(jsonDoc["data"]["productWidth"].GetFloat()) + " x " + FString::SanitizeFloat(jsonDoc["data"]["productHeight"].GetFloat()) + " x " + FString::SanitizeFloat(jsonDoc["data"]["productLength"].GetFloat());
		BearingName = FString::SanitizeFloat(jsonDoc["data"]["productBearing"].GetFloat()) + " kg";
		UserName = jsonDoc["data"]["adminUserName"].GetFString();
		UserAddress = jsonDoc["data"]["adminUserAddress"].GetFString();
		ImageUrl = jsonDoc["data"]["productImageUrl"].GetFString();
		ImagePath = jsonDoc["data"]["materialFolderUrl"].GetFString();
		UnitCode = jsonDoc["data"]["productCode"].GetFString();
		UnitURI = jsonDoc["data"]["productUrl"].GetFString();

		loadImageDetail();
		EventChangeUI();
		
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
}

void UCUI_mainDetails::loadImageDetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetURL(ImageUrl);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainDetails::loadImageCallback);
	HttpRequest->ProcessRequest();
}

void UCUI_mainDetails::loadImageCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) {

			const int32 count = Response->GetContentLength();
			TArray<uint8> tdata = Response->GetContent();

			// 设置文件路径
			FString url = FPaths::ProjectContentDir() + "/details.jpg";


//			if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*url))
//			{
				// 创建目录
//				VerifyOrCreateDirectory(FPaths::ProjectContentDir() + ImagePath);
				// 保存文件
				Tools::saveDataToFile(url, tdata);
//			}

			ImageTexture = cameraController->GetTexture(url);

			EventChangeImage();
		}
	}
}

bool UCUI_mainDetails::VerifyOrCreateDirectory(const FString& TestDir)
{
	TArray<FString> dirArr;
	TestDir.ParseIntoArray(dirArr, TEXT("/"), false);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString tmpDir = "";
	for (int32 i = 0; i < dirArr.Num(); ++i)
	{
		if (i == 0)
		{
			tmpDir += dirArr[i];
		}
		else
		{
			tmpDir = tmpDir + "/" + dirArr[i];
		}

		if (!PlatformFile.DirectoryExists(*tmpDir))
		{
			PlatformFile.CreateDirectory(*tmpDir);

			if (!PlatformFile.DirectoryExists(*tmpDir))
			{

			}
		}
	}

	return true;
}

void UCUI_mainDetails::log(FString myString)
{
	if (GEngine) 
	{
		myString = "UCUI_mainDetails::" + myString;
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Green, myString);
	}
}
