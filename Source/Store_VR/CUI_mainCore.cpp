// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_mainCore.h"
#include "CameraPlayerController.h"
#include "HAL/PlatformFilemanager.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageUtils.h"
#include "Public/AudioDevice.h"
#include "Public/Modules/ModuleInterface.h"


void UCUI_mainCore::initMainCore()
{
	// 加载二维码图片
	FString filePath = FPaths::ProjectContentDir() + "/Code.png";

	if (GetWorld())
	{
		CameraPC = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
		log("CameraPC");
		if (CameraPC)
		{
			log("textureBG");
			textureBG = GetTexture(filePath);
			if (textureBG)
			{
				log("ChangeCodeImage");
				ChangeCodeImage();
			}
		}
	}

	
}

void UCUI_mainCore::log(FString myString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 99.0f, FColor::Orange, myString);
	}
}

UTexture2D* UCUI_mainCore::GetTexture(FString url)
{
	// ===================================================

// 	IAudioEditorModule;
// 	FString testAbsoluteFilePath = "Image/test.wav";
// 	IPlatformFile& testPlatformFile = FPlatformFileManager::Get().GetPlatformFile();
// 
// 	if (testPlatformFile.FileExists(*testAbsoluteFilePath))
// 	{
// 		IAudioDeviceModule& AudioFormatModule = FModuleManager::LoadModuleChecked<IAudioDeviceModule>(FName("AudioDevice"));
// 		FAudioDevice* AudioDevice = AudioFormatModule.CreateAudioDevice();
// 
// 		TArray<uint8> testRawFileData;
// 		if (FFileHelper::LoadFileToArray(testRawFileData, *testAbsoluteFilePath))
// 		{
// 			AudioDevice->set
// 		}
// 	}

	

	



	// 	USoundWave;
	// 	SoundWave'/Game/Test/Sound/yulan-5b87c00c98f7a9118.yulan-5b87c00c98f7a9118';

		// ===================================================



	log(url);

	FString AbsoluteFilePath = url;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();


	//判断本地是否有文件
	if (PlatformFile.FileExists(*AbsoluteFilePath))
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		IImageWrapperPtr ImageWrapper;

		ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

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
