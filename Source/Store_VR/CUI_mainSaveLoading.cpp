// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_mainSaveLoading.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageUtils.h"
#include "CameraGameInstance.h"
#include "CameraPlayerController.h"
#include "Store_VRGameModeBase.h"
#include "CameraPawn.h"
#include "NetworkInterface.h"
#include "rapidjson/document.h"
#include "public/HAL/PlatformFilemanager.h"

using namespace rapidjson;

#define print(Format, ...) { UE_LOG(LogClass, Warning, Format, ##__VA_ARGS__); }
void UCUI_mainSaveLoading::initSaveLoading(FString json, FString phone)
{
	if (GetWorld())
	{
		cameraGI = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		cameraPawn = Cast<ACameraPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		gamemode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
		playController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
	}

	backgroundImage = playController->GetTexture("Image/Save_Loading.png");

	if (backgroundImage)
	{
		UpdataBackground();
	}

	roomTypeInfoGid = cameraGI->gid;
	gid = cameraGI->gidScheme;
	userPhone = phone;
	JsonActorAll = json;
	isSuccess = false;

	startSave();
}

void UCUI_mainSaveLoading::startSave()
{
	uploadConfigureFile();
}

void UCUI_mainSaveLoading::ProcessingImage()
{
	FString filePath = gamemode->GetFileDir(UE_BUILD_SHIPPING) + "HighresScreenshot00000";
	saveImage(filePath);
	playController->DeleteFile(filePath);

	UpdataImageEvent();
}

void UCUI_mainSaveLoading::uploadConfigureFile()
{
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
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainSaveLoading::uploadConfigureFileCompleteCallback);
	HttpRequest->ProcessRequest();

}


void UCUI_mainSaveLoading::uploadConfigureFileCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogClass, Warning, TEXT("uploadConfigureFileCompleteCallback"));

	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		isSuccess = false;
		EventUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			configureFile = SUploadItem();
			configureFile.gid = jsonDoc["gid"].GetInt();
			configureFile.fileName = jsonDoc["fileName"].GetFString();
			textFileUrl = jsonDoc["fileName"].GetFString();
			log("JosnCall gid : " + FString::FromInt(configureFile.gid) + "                fileName : " + configureFile.fileName);
			// 上传完成继续上传图片
			ProcessingImage();
		}
		else
		{
			// 上传失败
			isSuccess = false;
			EventUI();
		}

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		isSuccess = false;
		EventUI();
	}
	// UE_LOG(LogClass, Warning, TEXT("%s"), *MessageBody);
	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(TEXT("project.json"));
	print(TEXT("%s"), *MessageBody);
}

void UCUI_mainSaveLoading::uploadImage()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url.Append(UPLOAD_FILE);

	HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=blahblahsomeboundary");

	FString filePath = gamemode->GetFileDir(UE_BUILD_SHIPPING) + "HighresScreenshot00000.jpg";

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
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainSaveLoading::uploadImageCompleteCallback);
	HttpRequest->ProcessRequest();
}

void UCUI_mainSaveLoading::uploadImageCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	UE_LOG(LogClass, Warning, TEXT("uploadImageCompleteCallback"));
	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		isSuccess = false;
		EventUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			configureFile = SUploadItem();
			configureFile.gid = jsonDoc["gid"].GetInt();
			configureFile.fileName = jsonDoc["fileName"].GetFString();
			log("ImageCall gid : " + FString::FromInt(configureFile.gid) + "                fileName : " + configureFile.fileName);
			showImgUrl = jsonDoc["fileName"].GetFString();
			// 上传完成发送完成请求
			requestFinish();
		}
		else
		{
			// 上传失败
			//UE_LOG(LogClass, Warning, TEXT("上传失败"));
			isSuccess = false;
			EventUI();
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		isSuccess = false;
		EventUI();
	}
	// UE_LOG(LogClass, Warning, TEXT("%s"), *MessageBody);
	FString filePath = gamemode->GetFileDir(UE_BUILD_SHIPPING) + "HighresScreenshot00000.jpg";
	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*filePath);
	print(TEXT("%s"), *MessageBody);
}

void UCUI_mainSaveLoading::requestFinish()
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
	url += "&schemeName=";
	url += cameraGI->gid;
	url += "&houseTypeInfoGid=";
	url += cameraGI->houseGid;

	log("url is scheme : " + url);
	print(TEXT("url is scheme : %s"), *url);

	HttpRequest->SetURL(url);

	

// 	TMap<FString, FString> params;
// 	params.Add(TEXT("roomTypeInfoGid"), roomTypeInfoGid);
// 	params.Add(TEXT("userPhone"), userPhone);
// 	params.Add(TEXT("textFileUrl"), textFileUrl);
// 	params.Add(TEXT("showImgUrl"), showImgUrl);
// 	params.Add(TEXT("schemeName"), cameraGI->HouseTypeData.PropertyName_out);
// 
// 	// post 方法
// 	FString paramsString = getContentWithMap(params);
// 
// 	HttpRequest->SetContentAsString(paramsString);
// 
//  	log(paramsString);
//  	print(TEXT("paramsString : %s"), *paramsString);

	HttpRequest->SetVerb(TEXT("GET"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainSaveLoading::requestFinishCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_mainSaveLoading::requestFinishCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogClass, Warning, TEXT("requestFinishCompleteCallback"));

	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		isSuccess = false;
		EventUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			// 保存成功
			log("save success");
			isSuccess = true;
			EventUI();
		}
		else
		{
			// 保存失败
			log("save failure");
			isSuccess = false;
			EventUI();
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		isSuccess = false;
		EventUI();
	}
	UE_LOG(LogClass, Warning, TEXT("%s"), *MessageBody);
}

void UCUI_mainSaveLoading::saveImage(FString Path)
{
	// 图片输入路径
	FString filePath = Path + ".png";
	// 图片输出路径
	FString outPath = Path + ".jpg";
	// 颜色数组
	TArray<FColor> _colorArray;

	// 获取本地图片数据并将 颜色信息给与传入的值
	if (GetReadLocationImageData(&_colorArray, filePath))
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		if (ImageWrapper->SetRaw(_colorArray.GetData(), _colorArray.Num() * sizeof(FColor), 600, 600, ERGBFormat::RGBA, 8))
		{
			// 保存文件
			FFileHelper::SaveArrayToFile(ImageWrapper->GetCompressed(100), *outPath);
		}
	}
}

bool UCUI_mainSaveLoading::GetReadLocationImageData(TArray<FColor>* uncompressedFColor, FString& _path)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	TArray<uint8> RawFileData;

	// 读取图片数据
	if (FFileHelper::LoadFileToArray(RawFileData, *_path))
	{
		// 根据图片数据设置压缩
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
		{
			const TArray<uint8>* uncompressedRGBA;
			//获取图片原始数据
			if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, uncompressedRGBA))
			{
				// 把 uint8 转换成 FColor
				*uncompressedFColor = uint8ToFColor(*uncompressedRGBA);
				return true;
			}
		}
	}
	return false;
}

TArray<FColor> UCUI_mainSaveLoading::uint8ToFColor(const TArray<uint8> origin)
{
	TArray<FColor> uncompressedFColor;
	uint8 auxOrigin;
	FColor auxDst;

	// 本地图片颜色顺序 BGRA
	for (int i = 0; i < origin.Num(); i++) {
		auxOrigin = origin[i];
		auxDst.B = auxOrigin;
		i++;
		auxOrigin = origin[i];
		auxDst.G = auxOrigin;
		i++;
		auxOrigin = origin[i];
		auxDst.R = auxOrigin;
		i++;
		auxOrigin = origin[i];
		auxDst.A = auxOrigin;
		uncompressedFColor.Add(auxDst);
	}
	return uncompressedFColor;
}

void UCUI_mainSaveLoading::log(FString myString)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 99.0f, FColor::Magenta, myString);
	}
}

void UCUI_mainSaveLoading::ChangeGameInstance()
{
	if (GetWorld())
	{
		cameraGI = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		cameraGI->emap = EMaps::Select;
	}
}

FString UCUI_mainSaveLoading::getContentWithMap(TMap<FString, FString> params)
{
	FString paramsString = "";
	for (TPair<FString,FString>& element : params)
	{
		paramsString += element.Key;
		paramsString += "=";
		paramsString += *element.Value;
		paramsString += "&";
	}
	paramsString.RemoveAt(paramsString.Len() - 1);
	return paramsString;
}