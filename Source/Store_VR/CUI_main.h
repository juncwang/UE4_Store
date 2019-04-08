// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_select.h"
#include "Http.h"
#include "CUI_main.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_main : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// ��ͼ��ʼ��
	UFUNCTION(BlueprintImplementableEvent)
		void mainInitLoading();

	// ��ͼ��ʼ��
	UFUNCTION(BlueprintImplementableEvent)
		void mainCloseLoading();

	// �����ɫ
	class ACameraPawn* cameraPawn;
	class UCameraGameInstance* cameraInstance;
	class AStore_VRGameModeBase* gameMode;

	// ��ʼ�� UI
	UFUNCTION(BlueprintCallable)
	void initMain();

	// ��ͼ��ʼ��
	UFUNCTION(BlueprintImplementableEvent)
		void initMainBP();

	// �л��ӽǰ�ť�ص�
	UFUNCTION(BlueprintCallable)
		void ViewChange();

	// �л��ӽǰ�ť�ص�
	UFUNCTION(BlueprintCallable)
		void ViewFirstChange();

	// �Ƿ���ʾ���������¼�
	UFUNCTION(BlueprintImplementableEvent)
		void isShowHouseData();

	// ���� UI �ϻ��͵Ļ�������
	UFUNCTION(BlueprintImplementableEvent)
		void EventUpdataHouseBase();

	// ���»��ͷ��ť�¼�
	UFUNCTION(BlueprintImplementableEvent)
		void EventUpdataStyleBtn();

	// �ı� maps Ϊ loading ҳ��
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstanceToLoading();

	// ��ʼ���㵥��ȫ��ͼ
	UFUNCTION(BlueprintCallable)
		void PanoSimpleStart();

	// ��ʼ�������ȫ��ͼ
	UFUNCTION(BlueprintCallable)
		void PanoAllStart();

	// �������Ʒ��ť��ص�����
	UFUNCTION(BlueprintCallable)
		void AddMenuDown();

	// �򿪱���ѡ��
	UFUNCTION(BlueprintImplementableEvent)
		void openSaveTitle();

	// �������ʲ˵��¼�
	UFUNCTION(BlueprintImplementableEvent)
		void MaterialMenu();

	// �����������в˵��¼�
	UFUNCTION(BlueprintImplementableEvent)
		void CloseAllMenu();

	void log(FString myString);

	// ����ӿڷ��ʷ���

	// Ʒ��Ŀǰѡ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 BrandSelectGid;
	// ���Ŀǰѡ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 StyleSelectGid;
	// ����Ŀǰѡ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 TypeSelectGid;

	// ��ȡƷ���б���Ϣ
	// 1���Ҿ� 2������ 3��ǽ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 BRANDType;	// Ʒ��, ���͹�������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> BRANDName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> BRANDGid;
	UFUNCTION(BlueprintCallable)
		void SetBrandType(int32 num);
	void loadBRANDSchemeDetail();
	void LoadBRANDSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataPinPai();

	// ��ȡ����б���Ϣ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> STYLEName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> STYLEGid;
	void loadSTYLESchemeDetail();
	void LoadSTYLESchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataStyle();

	// ��ȡ��Ʒ�����б���Ϣ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> TYPEName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> TYPEGid;
	void loadTYPESchemeDetail();
	void LoadTYPESchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataType();


	// ��Ʒ�б�
	// ��ǰҳ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 curPage;
	// ����������������
	int32 pageSize;
	// ͼƬ��ַ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> imgURLArray;
	// ͼƬ��ַ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> _fileIdArray;
	// �ļ�·��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> FilePathArray;
	// �ļ�����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> FileNameArray;
	// ���ص�ַ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> DownPathArray;
	// �ļ� gid
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> gidArray;
	// ��һҳ��ť
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isPrevPage;
	// ��һҳ��ť
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isNextPage;
	// ������Ʒ�б�
	UFUNCTION(BlueprintCallable)
		void UpdataCommodity();
	UFUNCTION(BlueprintImplementableEvent)
		void UpadataComList();
	void loadCommoditySchemeDetail();
	void LoadCommoditySchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/************************************************************************/
	/* ��ʼ�� ҳ������                                                      */
	/************************************************************************/

	void initMainData();

	// ���������ȡ
	void loadHOUSEINFOSchemeDetail();
	void LoadHOUSEINFOSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// ����������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> areaNameArray;
	// �����С��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> areaSizeArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString deliveryArea;

	UFUNCTION(BlueprintImplementableEvent)
		void UpdataHouseInfo();

	// ��ȡ���
	void loadROOMLISTSchemeDetail();
	void LoadROOMLISTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// �ṹ�����-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FHouseType HouseType;


	// ���¼۸�
	void updataPrice(float _price);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float price;

	UFUNCTION(BlueprintImplementableEvent)
		void EventPrice();



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTexture2D* backgroundImage;

	class ACameraPlayerController* CameraPlayerController;

	UFUNCTION(BlueprintImplementableEvent)
		void UpdataBGImage();
};
