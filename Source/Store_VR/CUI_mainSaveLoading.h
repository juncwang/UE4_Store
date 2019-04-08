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

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString JsonActorAll;

	// ��Ա�ֻ�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString userPhone;

	// ����id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString roomTypeInfoGid;

	// ����id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString gid;

	// ������������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString textFileUrl;

	// ����ͼƬ����
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

	// �ı���Ϸʵ������, ��ͼ��ת level ʱ����
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstance();

	FString getContentWithMap(TMap<FString, FString> params);

private:
	SUploadItem configureFile;
	SUploadItem imageFile;
	
	
};
