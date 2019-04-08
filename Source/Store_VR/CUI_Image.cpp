// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_Image.h"
#include "CameraPlayerController.h"
#include "CameraGameInstance.h"
#include "Tools.h"
#include "HAL/PlatformFilemanager.h"
#include "Store_VRGameModeBase.h"



void UCUI_Image::InitHouseData(FString houseType_in)
{
	fHouseData_out.HouseName_out = codeTile_C;
	fHouseData_out.PropertyName_out = Community_C;
	fHouseData_out.HouseType_out = houseType_in;
	fHouseData_out.Building_out = HouseOutArea_C;
	fHouseData_out.Inside_out = HouseInArea_C;

	phoneNum = "";

	if (GetWorld())
	{
		CameraGameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
	}
}

void UCUI_Image::setProp(FString houseName, FString proName, int32 restNum, int32 rootNum, FString outArea, FString inArea, FString imageURL, EHouseType ehouseType, FString houseGid)
{
	codeTile_C = houseName;
	Community_C = proName;
	RestNum_C = restNum;
	RootNum_C = rootNum;
	HouseOutArea_C = outArea;
	HouseInArea_C = inArea;
	pathImage = imageURL;
	_houseGid = houseGid;

	if (ehouseType == EHouseType::House)
	{
		bHouseType = false;
	}
	else {
		bHouseType = true;
	}

	loadImageDetail();
}

void UCUI_Image::setPropType(FHouseType fHouseType)
{
	HouseType = fHouseType;
}

void UCUI_Image::setGid(FString inGid, FString inGidScheme, FString houseStyle, FString _phoneNum)
{
	gid = inGid;
	gidScheme = inGidScheme;
	fHouseData_out.HouseStyle = houseStyle;
	phoneNum = _phoneNum;
}

void UCUI_Image::ChangeGameInstance()
{
	UCameraGameInstance* CGInstance;
	if (GetWorld())
	{
		CGInstance = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		if (CGInstance != nullptr)
		{
			CGInstance->emap = EMaps::Loading;
		}
	}
}

void UCUI_Image::loadImageDetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetURL(pathImage);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_Image::loadImageCallback);
	HttpRequest->ProcessRequest();
}

void UCUI_Image::loadImageCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) {

			const int32 count = Response->GetContentLength();
			TArray<uint8> tdata = Response->GetContent();

			FString leftString;
			FString rightString;

			pathImage.Split("/", &leftString, &rightString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

			

			// 设置文件路径
			FString url = FPaths::ProjectContentDir() + "/images/" + rightString;


			if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*url))
			{
				// 创建目录
				VerifyOrCreateDirectory(FPaths::ProjectContentDir() + "/images/");
				// 保存文件
				Tools::saveDataToFile(url, tdata);
			}

			texture2d = cameraController->GetTexture(url);

			EventChange();
		}
		else {
			CameraGameMode->CreateNetErrorUI();
		}
	}
	else {
		CameraGameMode->CreateNetErrorUI();
	}
}

bool UCUI_Image::VerifyOrCreateDirectory(const FString& TestDir)
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
