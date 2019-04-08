// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_mainSaveLoading.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_mainSaveLoading : public UUserWidget
{
	GENERATED_BODY()

	struct SUploadItem
	{
		int32 gid;
		FString fileName;

	};
	
public: 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTexture2D* backgroundImage;

	UFUNCTION(BlueprintImplementableEvent)
		void UpdataBackground();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isSuccess;

	UFUNCTION(BlueprintImplementableEvent)
		void EventUI();

	UFUNCTION(BlueprintCallable)
		void initSaveLoading(FString json, FString phone);

	class AStore_VRGameModeBase* gamemode; 
	class ACameraPawn* cameraPawn;
	class UCameraGameInstance* cameraGI;
	class ACameraPlayerController* playController;

	// 方案数据
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString JsonActorAll;

	// 会员手机号码
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString userPhone;

	// 房型id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString roomTypeInfoGid;

	// 方案id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString gid;

	// 方案数据名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString textFileUrl;

	// 方案图片名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString showImgUrl;

	UFUNCTION(BlueprintCallable)
	void startSave();

	UFUNCTION(BlueprintCallable)
		void ProcessingImage();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdataImageEvent();

	void uploadConfigureFile();
	void uploadConfigureFileCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UFUNCTION(BlueprintCallable)
	void uploadImage();
	void uploadImageCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void requestFinish();
	void requestFinishCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void saveImage(FString Path);
	bool GetReadLocationImageData(TArray<FColor>* uncompressedFColor, FString& _path);
	TArray<FColor> uint8ToFColor(const TArray<uint8> origin);

	void log(FString myString);

	// 改变游戏实例属性, 蓝图跳转 level 时调用
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstance();

	FString getContentWithMap(TMap<FString, FString> params);

private:
	SUploadItem configureFile;
	SUploadItem imageFile;
	
	
};
