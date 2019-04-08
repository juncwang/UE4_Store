// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraPlayerController.h"
#include "HAL/PlatformFilemanager.h"
#include "Store_VRGameModeBase.h"
#include "CameraGameInstance.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageUtils.h"
#include "NetworkInterface.h"
#include "rapidjson/document.h"
#include "HAL/PlatformFilemanager.h"
#include "Tools.h"

using namespace rapidjson;

#define print(Format, ...) { UE_LOG(LogClass, Warning, Format, ##__VA_ARGS__); }
ACameraPlayerController::ACameraPlayerController()
{
}

void ACameraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	if (GetWorld())
	{
		CameraGameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
		CameraGI = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
	}
}



void ACameraPlayerController::UploadFileFromJPG()
{
	// 路径
	FString dir = "";
	// 文件路径
	FString filePath = "";
	// 文件名
	FString fileName = "";
	

	// 获取文件目录
	dir = CameraGameMode->GetFileDir(isSHIPPING());

	FilePathArray.Empty();

	for (int32 i = 0; i < 100; i++)
	{
		filePath = "";
		FString fileName = "HighresScreenshot000";
		if (i < 10)
		{
			fileName += "0";
		}
		filePath = dir + fileName + FString::FromInt(i);
		if (isVerifyFile(filePath))
		{
			FilePathArray.Push(filePath);
			OutPrint(filePath);
			SaveImageProcess(filePath);
			DeleteFile(filePath);
		}
		else {
			break;
		}
	}
	// 图片上传完成后从网络获取二维码图片并展示
	// 以下代码为模拟执行
	saveCount = 0;

	if (CameraGameMode->bAllPano)
	{
		allPanoDataArray.Empty();
		UploadAllPano();
	}
	else 
	{
		UploadSimPano(FilePathArray[0]);
	}
}

void ACameraPlayerController::UploadSimPano(FString path)
{
	uploadSimImage(path);
	deleteImagePath = path;
}

void ACameraPlayerController::UploadAllPano()
{
	if (FilePathArray.Num()>0)
	{
		uploadSimImage(FilePathArray[0]);
		deleteImagePath = FilePathArray[0];
		FilePathArray.RemoveAt(0);
	}
	else {
		// 调用 json 
		HotJson = "";
		// allPanoDataArray
		// FPointDataArray
		TArray<FUpateData> dataArray;

		for (int32 i =0; i < FPointDataArray.Num(); i++)
		{
			for (int32 j=0; j < FPointDataArray[i].otherArrowArray.Num(); j++)
			{
				FUpateData upateData;

				upateData.hotspotContent = FPointDataArray[i].otherArrowArray[j].arrowName;
				upateData.hotspotAth = FCString::Atof(*FPointDataArray[i].otherArrowArray[j].arrowYaw);
				upateData.panoramaInfoGid = allPanoDataArray[i].panoramaInfoGid;
				upateData.panoramaInfoNo = GetInfoNo(upateData.hotspotContent);

				dataArray.Push(upateData);
			}
		}

		HotJson += "[";

		for (int32 i = 0; i < dataArray.Num(); i++)
		{
			if (i != 0)
			{
				HotJson += ",";
			}

			HotJson += "{\"panoramaInfoGid\":";
			HotJson += FString::FromInt(dataArray[i].panoramaInfoGid);
			HotJson += ",\"panoramaInfoNo\":\"";
			HotJson += dataArray[i].panoramaInfoNo;
			HotJson += "\",\"hotspotContent\":\"";
			HotJson += dataArray[i].hotspotContent;
			HotJson += "\",\"hotspotAth\":";
			HotJson += FString::SanitizeFloat(dataArray[i].hotspotAth);
			HotJson += "}";
		}

		HotJson += "]";

		OutPrint(HotJson);
		print(TEXT("HotJson is : %s"), *HotJson);

		uploadConfigureFile();
	}
}

void ACameraPlayerController::uploadSimImage(FString path)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url.Append(PANO_UP_INTERFACE);

	HttpRequest->SetHeader("Content-Type", "multipart/form-data; boundary=blahblahsomeboundary");

	FString filePath = path + ".jpg";

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
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ACameraPlayerController::uploadSimImageCompleteCallback);
	HttpRequest->ProcessRequest();
}

void ACameraPlayerController::uploadSimImageCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogClass, Warning, TEXT("uploadImageCompleteCallback"));
	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		CameraGameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			int32 gid = jsonDoc["gid"].GetInt();
			FString fileName = jsonDoc["fileName"].GetFString();
			// 上传完成发送完成请求

			FString filePath = deleteImagePath + ".jpg";
			// 执行文件删除操作, 返回是否被删除
			FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*filePath);

			if (CameraGameMode->bAllPano) {
				allPanoSaveFinish(fileName);
			}
			else {
				simPanoSaveFinish(fileName);
			}

			
		}
		else
		{
			// 上传失败
			CameraGameMode->CreateNetErrorUI();
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		CameraGameMode->CreateNetErrorUI();
	}
}

void ACameraPlayerController::simPanoSaveFinish(FString gid)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += PANO_SAVE_INTERFACE;
	url += "?userPhone=";
	url += CameraGameMode->photoNum;
	url += "&memberSchemeInfGid=";
	url += CameraGI->gid;
	url += "&panoramaUrl=";
	url += gid;

	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ACameraPlayerController::simPanoSaveFinishCallback);

	HttpRequest->ProcessRequest();
}

void ACameraPlayerController::simPanoSaveFinishCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogClass, Warning, TEXT("requestFinishCompleteCallback"));

	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		CameraGameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			// 保存成功
			downloadMesh(jsonDoc["data"]["codeUrl"].GetFString());
			clickUrl = jsonDoc["data"]["htmlUrl"].GetFString();
		}
		else
		{
			CameraGameMode->CreateNetErrorUI();
			// 保存失败
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		CameraGameMode->CreateNetErrorUI();
	}
}

void ACameraPlayerController::allPanoSaveFinish(FString gid)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += PANO_SAVEMANY_INTERFACE;
	url += "?userPhone=";
	url += CameraGameMode->photoNum;
	url += "&memberSchemeInfGid=";
	url += CameraGI->gid;
	url += "&panoramaUrl=";
	url += gid;
	url += "&saveCount=";
	url += FString::FromInt(saveCount);

	saveCount++;

	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ACameraPlayerController::allPanoSaveFinishCallback);

	HttpRequest->ProcessRequest();
}

void ACameraPlayerController::allPanoSaveFinishCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogClass, Warning, TEXT("requestFinishCompleteCallback"));

	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		CameraGameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			// 保存成功
			FAllPanoData allPanoData;
			allPanoData.panoramaInfoGid = jsonDoc["data"]["panoramaInfoGid"].GetInt();
			allPanoData.panoramaInfoNo = jsonDoc["data"]["panoramaInfoNo"].GetFString();

			allPanoDataArray.Push(allPanoData);
			UploadAllPano();
		}
		else
		{
			CameraGameMode->CreateNetErrorUI();
			// 保存失败
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		CameraGameMode->CreateNetErrorUI();
	}
}

void ACameraPlayerController::downloadMesh(FString url)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ACameraPlayerController::downloadMeshComplete);
	HttpRequest->OnRequestProgress().BindUObject(this, &ACameraPlayerController::downloadRequestProgress);
	HttpRequest->ProcessRequest();
}

void ACameraPlayerController::downloadMeshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) {

			const int32 count = Response->GetContentLength();
			TArray<uint8> tdata = Response->GetContent();
			FString url = "";

			url = FPaths::ProjectContentDir() + "/Code.png";

			// 保存文件
			Tools::saveDataToFile(url, tdata);

			GameModeOver();
		}
		else
		{
			CameraGameMode->CreateNetErrorUI();
		}
	}
	else {
		CameraGameMode->CreateNetErrorUI();
	}
	
}

void ACameraPlayerController::downloadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived)
{
	FHttpResponsePtr HttpResponse = HttpRequest->GetResponse();

	if (HttpResponse.IsValid())
	{
		int32 TotalSize = HttpResponse->GetContentLength();
		float parent = (float)InBytesReceived / (float)TotalSize;
	}
	else {
		CameraGameMode->CreateNetErrorUI();
	}
}

void ACameraPlayerController::uploadConfigureFile()
{
	FPlatformFileManager::Get().GetPlatformFile().DeleteFile(TEXT("panoAllImage.json"));
	FString str = HotJson;
	FFileHelper::SaveStringToFile(str, TEXT("panoAllImage.json"), FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM);

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url.Append(PANO_HOTSPOT_INTERFACE);

	TArray<uint8> filedata;
	if (!FFileHelper::LoadFileToArray(filedata, TEXT("panoAllImage.json")))
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
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &ACameraPlayerController::uploadConfigureFileCompleteCallback);
	HttpRequest->ProcessRequest();
}

void ACameraPlayerController::uploadConfigureFileCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogClass, Warning, TEXT("uploadConfigureFileCompleteCallback"));

	FString MessageBody = "";
	// If HTTP fails client-side, this will still be called but with a NULL shared pointer!
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		CameraGameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			// 保存成功
			downloadMesh(jsonDoc["data"]["codeUrl"].GetFString());
			clickUrl = jsonDoc["data"]["htmlUrl"].GetFString();
		}
		else
		{
			CameraGameMode->CreateNetErrorUI();
			// 上传失败
		}

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		CameraGameMode->CreateNetErrorUI();
	}
	//UE_LOG(LogClass, Warning, TEXT("%s"), *MessageBody);
	print(TEXT("%s"), *MessageBody);
}

bool ACameraPlayerController::isVerifyFile(const FString& Path)
{
	FString filePath = Path + ".png";

	// 判断文件是否存在
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*filePath))
	{
		return true;
	}
	return false;
}

bool ACameraPlayerController::DeleteFile(const FString& Path)
{
	FString filePath = Path + ".png";
	// 执行文件删除操作, 返回是否被删除
	if (FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*filePath))
	{
		return true;
	}
	return false;
}

void ACameraPlayerController::SaveImageProcess(const FString& Path)
{
	// 图片输入路径
	FString filePath = Path + ".png";
	// 图片输出路径
	FString outPath = Path + ".jpg";
	// 颜色数组
	TArray<FColor> uncompressedFColor;

	// 获取本地图片数据并将 颜色信息给与传入的值
	if (GetReadLocationImageData(&uncompressedFColor, filePath))
	{
		// 使用异步保存图片
		SaveImage(uncompressedFColor, outPath);
	}
}

bool ACameraPlayerController::GetReadLocationImageData(TArray<FColor>* uncompressedFColor,FString& _path)
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

bool ACameraPlayerController::isSHIPPING()
{
	if (UE_BUILD_SHIPPING)
	{
		return true;
	}
	return false;
}

TArray<FColor> ACameraPlayerController::uint8ToFColor(const TArray<uint8> origin)
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

void ACameraPlayerController::SaveImageAsync(TArray<FColor> _colorArray, FString _path)
{
	Async<void>(EAsyncExecution::ThreadPool, [=]() 
	{ 
		SaveImage(_colorArray, _path);
	});
}

void ACameraPlayerController::SaveImage(TArray<FColor> _colorArray, FString _path)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
	if (ImageWrapper->SetRaw(_colorArray.GetData(), _colorArray.Num() * sizeof(FColor), 4096, 2048, ERGBFormat::RGBA, 8))
	{
		// 保存文件
		FFileHelper::SaveArrayToFile(ImageWrapper->GetCompressed(100), *_path);
	}
}

UTexture2D* ACameraPlayerController::GetTexture(FString url)
{
	FString AbsoluteFilePath = url;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	//判断本地是否有文件
	if (PlatformFile.FileExists(*AbsoluteFilePath))
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		IImageWrapperPtr ImageWrapper;
		
		FString imgName = AbsoluteFilePath.Right(3);
		if (imgName.Equals("png") || imgName.Equals("PNG"))
		{
			ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		}
		else
		{
			ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		}

		TArray<uint8> RawFileData;
		if (FFileHelper::LoadFileToArray(RawFileData, *AbsoluteFilePath))
		{
			if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
			{
				const TArray<uint8>* UncompressedBGRA = NULL;
				if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
				{
					UTexture2D* mytex = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
					void* TextureData = mytex->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
					FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
					mytex->PlatformData->Mips[0].BulkData.Unlock();

					mytex->UpdateResource();

					return mytex;

				}
			}
		}
	}
	return nullptr;
}

void ACameraPlayerController::OutPrint(FString myString)
{
	if (GEngine)
	{
		myString = "ACameraPlayerController::" + myString;
		GEngine->AddOnScreenDebugMessage(-1, 10.0, FColor::Green, myString);
	}
}

void ACameraPlayerController::GameModeOver()
{
	CameraGameMode->removeMainPanoLoading();
	CameraGameMode->mainCoreUI();
}

FString ACameraPlayerController::GetInfoNo(FString arrowName)
{
	for (int32 i = 0; i < FPointDataArray.Num(); i++)
	{
		if (arrowName.Equals(FPointDataArray[i].pointName))
		{
			return allPanoDataArray[i].panoramaInfoNo;
		}
	}

	return "";
}

void ACameraPlayerController::clearImage()
{
	// 路径
	FString dir = "";
	// 文件路径
	FString filePath = "";

	// 获取文件目录
	dir = CameraGameMode->GetFileDir(isSHIPPING());

	for (int32 i = 0; i < 100; i++)
	{
		filePath = "";
		FString fileName = "HighresScreenshot000";
		if (i < 10)
		{
			fileName += "0";
		}
		filePath = dir + fileName + FString::FromInt(i);
		FString filejpg = filePath + ".jpg";
		FString filepng = filePath + ".png";

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*filejpg))
		{
			FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*filejpg);
		}

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*filepng))
		{
			FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*filepng);
		}
	}
}

