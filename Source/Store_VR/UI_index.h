// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "UI_index.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UUI_index : public UUserWidget
{
	GENERATED_BODY()
	
public:

	class ACameraPlayerController* CameraPlayerController;
	class AStore_VRGameModeBase* gameMode;
	class UCameraGameInstance* cgi;

	// 版本号 本地版本号 网络版本号
	FString localVersion;
	FString netVersion;

	// 下载地址
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString downloadLink;

	// 背景图片
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* backgroundImage;

	// 初始化 UI_index 组件
	UFUNCTION(BlueprintCallable)
		void initIndex();

	// 检查是否有新版本
	void checkVersion();

	// 比对版本号
	void ComparedVersion(FString version);

	// 触发更新事件
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataVersion();

	// 触发图片加载事件
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataBGImage();

	// 读取软件版本信息
	void loadVersionSchemeDetail();
	void LoadVersionSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	void log(FString myString);
	
};
