// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_mainImage.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformFilemanager.h"
#include <fstream>
#include <iomanip>
#include "UMG.h"
#include <stdio.h>
#include "Tools.h"
#include "Store_VRGameModeBase.h"

using namespace std;

#define print(Format, ...) { UE_LOG(LogClass, Warning, Format, ##__VA_ARGS__); }
void UCUI_mainImage::init_mainImage(FString imageUrl, FString filePath, FString downPath, FString fileName, int32 gid, FString fileId)
{

	if (GetWorld())
	{
		gameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
		bDowning = false;
	}

	bZip = true;

	print(TEXT("init_mainImage %s"), *filePath);
	_filePath = filePath;
	_zipPath = downPath;
	_fileName = fileName;
	_fileId = fileId;
	if (!VerifyOrCreateDirectory(FPaths::ProjectContentDir() + filePath))
	{
		print(TEXT("VerifyOrCreateDirectory %s"), *(FPaths::ProjectContentDir() + filePath));
	}
 	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
// 	FString imageFilePath = FPaths::ProjectContentDir() + filePath + "/preview.jpg";
// 	if (PlatformFile.FileExists(*imageFilePath))
// 	{
// 		print(TEXT("FileExists:%s"), *imageFilePath);
// 		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
// 		// Note: PNG format.  Other formats are supported
// 		IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
// 		UImage* image = Cast<UImage>(GetWidgetFromName(FName("imageModeBG")));
// 		TArray<uint8> RawFileData;
// 		if (FFileHelper::LoadFileToArray(RawFileData, *imageFilePath))
// 		{
// 			if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
// 			{
// 				const TArray<uint8>* UncompressedBGRA = NULL;
// 				if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
// 				{
// 					UTexture2D* mytex = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
// 					void* TextureData = mytex->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
// 					FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
// 					mytex->PlatformData->Mips[0].BulkData.Unlock();
// 
// 					mytex->UpdateResource();
// 
// 					image->SetVisibility(ESlateVisibility::Visible);
// 					image->SetBrushFromTexture(mytex);
// 
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		loadImageDetail(imageUrl);
// 	}

	loadImageDetail(imageUrl);

	FString zipFilePath = FPaths::ProjectContentDir() + _filePath + "/" + _fileName + ".uasset";
	FString filePathId = FPaths::ProjectContentDir() + _filePath + "/" + fileId;
	print(TEXT("zipFilePath %s"), *zipFilePath);

	if (PlatformFile.FileExists(*zipFilePath) && PlatformFile.FileExists(*filePathId))
	{
		// 文件存在直接加载uassets
		eventUpdataUI();
	}
}

void UCUI_mainImage::loadImageDetail(FString url)
{
	print(TEXT("%s"), *url)
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainImage::loadImageCallback);
	HttpRequest->ProcessRequest();
}

void UCUI_mainImage::loadImageCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	 	if (Response.IsValid())
	 	{
	 		if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) {
	 
	 			const int32 count = Response->GetContentLength();
	 			TArray<uint8> tdata = Response->GetContent();
	 
	 			// 设置文件路径
	 			FString url = FPaths::ProjectContentDir() + _filePath + "/preview.jpg";
	 
	 			// 创建目录
	 			VerifyOrCreateDirectory(FPaths::ProjectContentDir() + _filePath);
	 			// 保存文件
				Tools::saveDataToFile(url, tdata);

				IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
				// Note: PNG format.  Other formats are supported
				IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
				UImage* image = Cast<UImage>(GetWidgetFromName(FName("imageModeBG")));
				TArray<uint8> RawFileData;
				if (FFileHelper::LoadFileToArray(RawFileData, *url))
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

							image->SetVisibility(ESlateVisibility::Visible);
							image->SetBrushFromTexture(mytex);

						}
					}
				}
			}
			else {
				gameMode->CreateNetErrorUI();
			}
		}
		else {
			gameMode->CreateNetErrorUI();
		}
}

bool UCUI_mainImage::VerifyOrCreateDirectory(const FString& TestDir)
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
			print(TEXT("tmpDir:%s"), *tmpDir);

			if (!PlatformFile.DirectoryExists(*tmpDir))
			{

			}
		}
	}

	return true;
}

void UCUI_mainImage::onButtonClicked()
{
	if (!bDowning) {
		print(TEXT("onClicked"));
		if (!VerifyOrCreateDirectory(FPaths::ProjectContentDir() + _filePath))
		{
			print(TEXT("VerifyOrCreateDirectory %s"), *(FPaths::ProjectContentDir() + _filePath));
		}
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		FString zipFilePath = FPaths::ProjectContentDir() + _filePath + "/" + _fileName + ".uasset"; // 改成对应的uassets文件名
		FString filePathId = FPaths::ProjectContentDir() + _filePath + "/" + _fileId;

		if (PlatformFile.FileExists(*zipFilePath) && PlatformFile.FileExists(*filePathId))
		{
			// 文件存在直接加载uassets
			if (bZip)
			{
				eventModeOrMater();
			}
		}
		else
		{
			bZip = false;
			// 文件不存在开始下载
			downloadMesh(_zipPath);
		}
	}

	
}

void UCUI_mainImage::downloadMesh(FString url)
{
	print(TEXT("downloadMesh"));
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("GET"));
	bDowning = true;
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_mainImage::downloadMeshComplete);
	HttpRequest->OnRequestProgress().BindUObject(this, &UCUI_mainImage::downloadRequestProgress);
	HttpRequest->ProcessRequest();
}

void UCUI_mainImage::downloadMeshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	print(TEXT("downloadMeshComplete"));
	if (Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) {

			const int32 count = Response->GetContentLength();
			TArray<uint8> tdata = Response->GetContent();

			// 设置文件路径
			FString url = FPaths::ProjectContentDir() + _filePath + "/mesh.zip";

			// 创建目录
			VerifyOrCreateDirectory(FPaths::ProjectContentDir() + _filePath);


			FString fileId = FPaths::ProjectContentDir() + _filePath + "/" + _fileId;

			log("path " + fileId);
			log("id " + _fileId);

			FFileHelper::SaveStringToFile(_fileId, *fileId);
			// 保存文件
			Tools::saveDataToFile(url, tdata);


			//QString cmd0 = "../../../../7-Zip/7z.exe x " + file + ".rar -o\"" + targetDir + "\"";

			FString cmd = "7z.exe x " + url + " -o\"" + FPaths::ProjectContentDir() + _filePath;
			print(TEXT("cmd:%s"), *cmd);
			Tools::system_hide(cmd.GetCharArray().GetData());

			log("Tools::system_hide(cmd.GetCharArray().GetData());");

			bZip = true;
			bDowning = false;
		}
		else {
			gameMode->CreateNetErrorUI();
			bDowning = false;
		}
	}
	else {
		gameMode->CreateNetErrorUI();
		bDowning = false;
	}
}

void UCUI_mainImage::downloadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived)
{

		FHttpResponsePtr HttpResponse = HttpRequest->GetResponse();

		if (HttpResponse.IsValid())
		{
			int32 TotalSize = HttpResponse->GetContentLength();
			float parent = (float)InBytesReceived / (float)TotalSize;
			print(TEXT("%.2f"), parent);
			loadingNum = 120.0f * parent;
			eventloading();
		}
		else {
			gameMode->CreateNetErrorUI();
		}
}

void UCUI_mainImage::log(FString myString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 99, FColor::Blue, myString);
	}
}

