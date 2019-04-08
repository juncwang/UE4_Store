// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_loading.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_loading : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void mainInitLoading();

	UFUNCTION(BlueprintCallable)
		void mainCloseLoading();

	// ������
	class ACameraPlayerController* CameraPlayerController;

	// ��Ϸ���
	class UCameraGameInstance* cameraGameInstance;

	// ��Ϸģʽ
	class AStore_VRGameModeBase* gameMode;

	// ÿ��ģ�͵Ľ���ֵ
	float simModelNum;

	// �ܽ��ȴ�С
	int32 numSize;

	// ����ģ��ʱ, ��һ�μ��˶���
	float prevNum;

	// ��ȡ�ĵ�ǰ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float numLoading;

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString textLoading;

	// ����ͼƬ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTexture2D* backgroundImage;

	// ���� GID 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gid;

	// ���ͷ��� GID 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gidScheme;

	// ��ʼ�� UI ����
	UFUNCTION(BlueprintCallable)
		void initLoading();
	
	// �ı�������¼�
	UFUNCTION(BlueprintImplementableEvent)
		void EventChange();
	
	// �ı䱳�����¼�
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataBG();

	// �鿴�ļ��Ƿ����
	bool isVerifyFile(const FString& Path);

	// ���� CGI �ļ��� UI ����
	UFUNCTION(BlueprintCallable)
	void ChangeGameInstance();

	void log(FString myString);

	/************************************************************************/
	/* ���粿��                                                             */
	/************************************************************************/

	// �Ƿ����� json �ļ�
	bool isJsonFile;
	// �Ƿ����� map �ļ�
	bool isMapFile;

	// �ļ���
	FString _FileName;
	// �ļ�·��
	FString _FilePath;
	// �ļ����ص�ַ
	FString _FileDownUrl;
	// �ļ��汾��
	FString _FileId;

	// map �ļ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString _mapFileName;
	// map �ļ�·��
	FString _mapFilePath;
	// map �ļ����ص�ַ
	FString _mapFileDownUrl;

	// ģ�� gid ��
	TArray<FString> _modelGidArray;
	// ģ�� �ļ��� ��
	TArray<FString> _modelFileNameArray;
	// ģ�� �ļ�·�� ��
	TArray<FString> _modelFilePathArray;
	// ģ�� �ļ����ص�ַ ��
	TArray<FString> _modelFileDownUrlArray;
	// ģ�Ͱ汾����
	TArray<FString> _modelFileIDArray;

	// ���� gid ��
	TArray<FString> _materialGidArray;
	// ���� �ļ��� ��
	TArray<FString> _materialFileNameArray;
	// ���� �ļ�·�� ��
	TArray<FString> _materialFilePathArray;
	// ���� �ļ����ص�ַ ��
	TArray<FString> _materialFileDownUrlArray;
	// ���ʰ汾����
	TArray<FString> _materialFileIDArray;

	// ��ȡ���ͷ���json�����ļ�
	void GetSchemeJson();
	void loadSCHEMEINFODetail();
	void LoadSCHEMEINFODetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// ����ÿ����Ʒ����ռ��
	void CalculatedRatio();

	// ��ȡ model �ڵ�����
	void StartModeData();

	// ��ȡ model �ڵ�ģ������
	void loadMODELINFDetail(FString gidArray1, FString gidArray2);
	void LoadMODELINFDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// ��ȡ material �ڵ�����
	void StartMaterialData();

	// ��ȡ material �ڵ�ģ������
	void loadMATERIALINFODetail(FString gidArray);
	void LoadMATERIALINFODetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// ����ļ��Ƿ����
	void CleckFile();

	// ��ȡ��������
	void loadROOMTYPEDetail();
	void LoadROOMTYPEDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// ���������ļ�����
	void downloadMesh(FString url);
	void downloadMeshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void downloadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived);

	// �����ļ������ط���
	bool VerifyOrCreateDirectory(const FString& TestDir);

	// �򿪳���
	UFUNCTION(BlueprintImplementableEvent)
	void openMap();
};
