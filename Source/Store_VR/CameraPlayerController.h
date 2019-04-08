// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PanoActor.h"
#include "GameFramework/PlayerController.h"
#include "Http.h"
#include "CameraPlayerController.generated.h"

// id ����
USTRUCT(BlueprintType)
struct FAllPanoData {

	GENERATED_BODY()

	// ȫ��ͼid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 panoramaInfoGid;

	// ȫ��ͼ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString panoramaInfoNo;
};

// �ϴ� ����
USTRUCT(BlueprintType)
struct FUpateData {

	GENERATED_BODY()

	// ȫ��ͼid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 panoramaInfoGid;

	// ָ��ȫ��ͼ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString panoramaInfoNo;

	// ָ�򷿼�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString hotspotContent;

	// ָ�򷿼�Ƕ�
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

	// ��Ҫɾ����ͼƬ��ַ
	FString deleteImagePath;

	// �����ļ��Ƿ����
	bool isVerifyFile(const FString& dir);

	// ɾ���Ѵ����ļ�
	bool DeleteFile(const FString& filePath);

	// ����ͼƬʱ�����еĹ���
	void SaveImageProcess(const FString& Path);

	// ��ȡ����ͼƬ, ����ȡͼƬ����ɫ����
	bool GetReadLocationImageData(TArray<FColor>* uncompressedFColor, FString& _path);

	// �����Ƿ�Ϊ����ģʽ
	bool isSHIPPING();

	// �� uint8 ת�� FColor
	TArray<FColor> uint8ToFColor(const TArray<uint8> origin);

	// ʹ���첽����ͼƬ
	void SaveImageAsync(TArray<FColor> _colorArray, FString _path);

	// ����ͼƬ
	void SaveImage(TArray<FColor> _colorArray, FString _path);

	// ��ȡ Texture2d ͼƬ
	UTexture2D* GetTexture(FString url);

	// ӫ����ӡ �ַ���
	void OutPrint(FString myString);


	// ȫ��ͼ����� =========================================================================��

	// ��λ����
	TArray<FPointData> FPointDataArray;

	// �ϴ� jpg ͼƬ�ļ�
	void UploadFileFromJPG();

	// �ϴ�����ȫ��ͼ
	void UploadSimPano(FString path);
	// �ϴ�����ȫ��ͼ
	void UploadAllPano();

	void uploadSimImage(FString path);
	void uploadSimImageCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void simPanoSaveFinish(FString gid);
	void simPanoSaveFinishCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void allPanoSaveFinish(FString gid);
	void allPanoSaveFinishCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// ���������ļ�����
	void downloadMesh(FString url);
	void downloadMeshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void downloadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived);

	// �ϴ������ļ�
	void uploadConfigureFile();
	void uploadConfigureFileCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// ���� gamemode ����ȫ��ͼ����
	void GameModeOver();


	// ��ȡָ�򷿼�ı��
	FString GetInfoNo(FString arrowName);

	// �ļ�·����
	TArray<FString> FilePathArray;


	UFUNCTION(BlueprintCallable)
		void clearImage();
};
