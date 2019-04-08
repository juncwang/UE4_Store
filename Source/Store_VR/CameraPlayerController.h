// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PanoActor.h"
#include "GameFramework/PlayerController.h"
#include "Http.h"
#include "CameraPlayerController.generated.h"

// id 数据
USTRUCT(BlueprintType)
struct FAllPanoData {

	GENERATED_BODY()

	// 全景图id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 panoramaInfoGid;

	// 全景图编号
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString panoramaInfoNo;
};

// 上传 数据
USTRUCT(BlueprintType)
struct FUpateData {

	GENERATED_BODY()

	// 全景图id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 panoramaInfoGid;

	// 指向全景图编号
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString panoramaInfoNo;

	// 指向房间名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString hotspotContent;

	// 指向房间角度
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float hotspotAth;
};

/**
 * 
 */
UCLASS()
class STORE_VR_API ACameraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	

	class AStore_VRGameModeBase* CameraGameMode;
	class UCameraGameInstance* CameraGI;

	TArray<FAllPanoData> allPanoDataArray;

	FString HotJson;

	int32 saveCount;
	
public:

	ACameraPlayerController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString clickUrl;

	virtual void BeginPlay() override;

	// 需要删除的图片地址
	FString deleteImagePath;

	// 返回文件是否存在
	bool isVerifyFile(const FString& dir);

	// 删除已存在文件
	bool DeleteFile(const FString& filePath);

	// 保存图片时需运行的过程
	void SaveImageProcess(const FString& Path);

	// 读取本地图片, 并获取图片的颜色数组
	bool GetReadLocationImageData(TArray<FColor>* uncompressedFColor, FString& _path);

	// 返回是否为生产模式
	bool isSHIPPING();

	// 把 uint8 转成 FColor
	TArray<FColor> uint8ToFColor(const TArray<uint8> origin);

	// 使用异步保存图片
	void SaveImageAsync(TArray<FColor> _colorArray, FString _path);

	// 保存图片
	void SaveImage(TArray<FColor> _colorArray, FString _path);

	// 获取 Texture2d 图片
	UTexture2D* GetTexture(FString url);

	// 荧屏打印 字符串
	void OutPrint(FString myString);


	// 全景图最后功能 =========================================================================》

	// 点位数据
	TArray<FPointData> FPointDataArray;

	// 上传 jpg 图片文件
	void UploadFileFromJPG();

	// 上传单张全景图
	void UploadSimPano(FString path);
	// 上传多张全景图
	void UploadAllPano();

	void uploadSimImage(FString path);
	void uploadSimImageCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void simPanoSaveFinish(FString gid);
	void simPanoSaveFinishCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void allPanoSaveFinish(FString gid);
	void allPanoSaveFinishCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 网络下载文件方法
	void downloadMesh(FString url);
	void downloadMeshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void downloadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived);

	// 上传配置文件
	void uploadConfigureFile();
	void uploadConfigureFileCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 调用 gamemode 结束全景图程序
	void GameModeOver();


	// 获取指向房间的编号
	FString GetInfoNo(FString arrowName);

	// 文件路径组
	TArray<FString> FilePathArray;


	UFUNCTION(BlueprintCallable)
		void clearImage();
};
