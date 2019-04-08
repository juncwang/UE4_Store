// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "http.h"
#include "CUI_mainSaveMobile.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_mainSaveMobile : public UUserWidget
{
	GENERATED_BODY()
	
		struct SUploadItem
	{
		int32 gid;
		FString fileName;

	};
public:

	class AStore_VRGameModeBase* gamemode;

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
		void SetJsonActorAll(FString phone);

	UFUNCTION(BlueprintCallable)
	void startSave();

	void uploadConfigureFile();
	void uploadConfigureFileCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void uploadImage();
	void uploadImageCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void requestFinish();
	void requestFinishCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	SUploadItem configureFile;
	SUploadItem imageFile;

	
};
