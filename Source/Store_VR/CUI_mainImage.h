// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_mainImage.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_mainImage : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bZip;

	bool bDowning;

	// 初始化 UI : 用于 图片加载, 检测文件是否存在
	UFUNCTION(BlueprintCallable)
	void init_mainImage(FString imageUrl, FString filePath, FString downPath, FString fileName, int32 gid, FString fileId);

	UFUNCTION(BlueprintCallable)
	void onButtonClicked();

	// 如果有文件更新 UI
	UFUNCTION(BlueprintImplementableEvent)
		void eventUpdataUI();

	// 新加模型或更换材质事件
	UFUNCTION(BlueprintImplementableEvent)
		void eventModeOrMater();

	// 新加模型或更换材质事件
	UFUNCTION(BlueprintImplementableEvent)
		void eventloading();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float loadingNum;

	class AStore_VRGameModeBase* gameMode;

	// 获取网络图片到本地显示
	void loadImageDetail(FString url);
	void loadImageCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	bool VerifyOrCreateDirectory(const FString& TestDir);

	void downloadMesh(FString url);
	void downloadMeshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void downloadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived);

private:
	FString _filePath;
	FString _zipPath;
	FString _fileName;
	FString _fileId;
	
	void log(FString myString);
};
