// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraGameInstance.h"
#include "Kismet/GameplayStatics.h"


UCameraGameInstance::UCameraGameInstance()
{
	emap = EMaps::Index;
}


void UCameraGameInstance::log()
{
	FString out;
	switch (emap)
	{
	case EMaps::Index:
		out = "Index";
		break;
	case EMaps::Select:
		out = "Select";
		break;
	case EMaps::Loading:
		out = "Loading";
		break;
	case EMaps::MainPage:
		out = "MainPage";
		break;
	default:
		break;
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 9.0, FColor::Green, out);
	}
}

// USoundWave* UCameraGameInstance::GetSoundWaveFromFile()
// {
// 	FString filePath = "Sound/BackgroundSound.wav";
// 
// 	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());
// 
// 	if (!sw)
// 		return nullptr;
// 
// 	TArray < uint8 > rawFile;
// 
// 	FFileHelper::LoadFileToArray(rawFile, filePath.GetCharArray().GetData());
// 	FWaveModInfo WaveInfo;
// 
// 	if (WaveInfo.ReadWaveInfo(rawFile.GetData(), rawFile.Num()))
// 	{
// 		sw->InvalidateCompressedData();
// 
// 		sw->RawData.Lock(LOCK_READ_WRITE);
// 		void* LockedData = sw->RawData.Realloc(rawFile.Num());
// 		FMemory::Memcpy(LockedData, rawFile.GetData(), rawFile.Num());
// 		sw->RawData.Unlock();
// 
// 		int32 DurationDiv = *WaveInfo.pChannels * *WaveInfo.pBitsPerSample * *WaveInfo.pSamplesPerSec;
// 		if (DurationDiv)
// 		{
// 			sw->Duration = *WaveInfo.pWaveDataSize * 8.0f / DurationDiv;
// 		}
// 		else
// 		{
// 			sw->Duration = 0.0f;
// 		}
// 		sw->SampleRate = *WaveInfo.pSamplesPerSec;
// 		sw->NumChannels = *WaveInfo.pChannels;
// 		sw->RawPCMDataSize = WaveInfo.SampleDataSize;
// 		sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
// 	}
// 	else {
// 		return nullptr;
// 	}
// 
// 	return sw;
// }

// void UCameraGameInstance::playSound()
// {
// 	USoundWave* sw = GetSoundWaveFromFile();
// 
// 	if (sw)
// 	{
// 		sw->bLooping = true;
// 		UGameplayStatics::PlaySound2D(GetWorld(), sw, 0.3f);
// 	}
// }
