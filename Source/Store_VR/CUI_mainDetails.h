// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_mainDetails.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_mainDetails : public UUserWidget
{
	GENERATED_BODY()
	
public:

	class ACameraPawn* cameraPawn;
	class AStore_VRGameModeBase* gameMode;
	class ACameraPlayerController* cameraController;

	// 产品编码
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UnitCode;

	// 产品链接地址
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UnitURI;

	// 产品单价
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UnitPrice;

	// 品牌及商品名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BrandAndProductName;

	// 风格名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString StyleName;

	// 材质名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString MaterialName;

	// 商品尺寸
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString SizeName;

	// 商品重量
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BearingName;

	// 商家名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UserName;

	// 商家地址
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UserAddress;

	// 图片下载地址
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString ImageUrl;

	// 图片保存路劲
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString ImagePath;

	// 更改 UI 事件
	UFUNCTION(BlueprintImplementableEvent)
		void EventChangeUI();

	// 图片文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTexture2D* ImageTexture;

	// 更改 图片 事件
	UFUNCTION(BlueprintImplementableEvent)
		void EventChangeImage();

	// 关闭 事件
	UFUNCTION(BlueprintImplementableEvent)
		void EventClose();

	FString gid;

	// 回调函数, 请求数据
	UFUNCTION(BlueprintCallable)
		void initDetails();

	// 请求商品详情数据
	void loadPRODUCTSchemeDetail();
	void LoadPRODUCTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 图片获取
	void loadImageDetail();
	void loadImageCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	bool VerifyOrCreateDirectory(const FString& TestDir);
	
	void log(FString myString);
};
