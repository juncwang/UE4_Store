// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_select.generated.h"

// ͼƬ��ʾ����������
UENUM()
enum class EHouseType :uint8
{
	House,		// ��������
	Design,		// �������
};

// ��������������
USTRUCT(BlueprintType)
struct FSearchData
{
	GENERATED_BODY()

	// ��ť������ʾ������, �����ͨ�������ݽ�������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString searchBtn;

	// �����ť������� url ��ַ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString url;
};

// ���ͷ���ͼ
// �������û�����������, ��ô�����ַ��� ""
USTRUCT(BlueprintType)
struct FHouseType
{
	GENERATED_BODY()

	// �ִ�������� gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XDGid;

	// ���� ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XDSchemeGid;

	// �ִ�������ݷ�� id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XDConfigStyleGid;

	// ����ʽ������� gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XZSGid;

	// ���� ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XZSSchemeGid;

	// ����ʽ������ݷ�� id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XZSConfigStyleGid;

	// ��ŷ������� gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 BOGid;

	// ���� ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 BOSchemeGid;

	// ��ŷ������ݷ�� id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 BOConfigStyleGid;

	// С��������� gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XMGid;

	// ���� ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XMSchemeGid;

	// С��������ݷ�� id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XMConfigStyleGid;

	// �����Ƿ������ gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 DNYGid;

	// ���� ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 DNYSchemeGid;

	// �����Ƿ�����ݷ�� id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 DNYConfigStyleGid;

};

// չʾ����������
USTRUCT(BlueprintType)
struct FHouseData
{
	GENERATED_BODY()

	// ��ͼ�������ļ���ַ url �� �����ļ�
	// ��� EHouseType Ϊ House ����, ��ô���ؿ��ַ��� "" 
	// ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString MapsConfiguration;

	// ��ͼ������, ��ͼͨ�����ƽ����л� ��ά��ͼ
	// ��� EHouseType Ϊ House ����, ��ô���ؿ��ַ��� ""
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString pathMap;

	// ��ͼ�ڱ��ص�λ��, ���ڳ����鱾���Ƿ��е�ͼ�ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString CleckPathMap;

	// map �ı���ͼƬ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString pathImage;

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString codeTile;

	// ����¥��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString Community;

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString HouseType;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString HouseOutArea;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString HouseInArea;
};

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_select : public UUserWidget
{
	GENERATED_BODY()
	
public:

	class AStore_VRGameModeBase* gameMode;

	// ������ȡ�ص��������ֵ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FSearchData> searchDataArray;

	// �������ȡ�ķ�������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FHouseData> houseDataArray;

	// ���� gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> HouseGidArray;

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> HouseNameArray;

	// ¥������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> PropertyNameArray;

	// �����������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int32> RestaurantNumArray;

	// ������������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int32> RoomNumArray;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> OutAreaArray;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> InAreaArray;

	// ͼƬ��ַ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> ImageUrlArray;

	// �����������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FHouseType> houseTypeArray;

	// ��״̬Ϊ Design ʱ ��Ҫ��ȡ������===========================================================================================

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	// ���� gid ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> gidArray;

	// ���� gid ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> phoneNumArray;

	// ���ͷ��� gid ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> gidSchemeArray;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> HouseStyleArray;

	// ===============================================================================================================================

	// �ı�������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FText textName;

	// ��ǰҳ��
	int32 pageNum;

	// ��������
	int32 dataMax;

	// ���ҳ��
	int32 pageMax;

	// ��Ҫ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EHouseType eHouse;

	// ������������
	bool bSearch;

	// ���������Ƿ�Ϊ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isSearchData;

	// ��ǰҳ����������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 pageNowNum;

	// �Ƿ���ʾ��ǰ��ť
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isShowUpBtn;

	// �Ƿ���ʾ���ť
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isShowNetBtn;

	// �����ؼ���
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString searchKeyword;

	// ������������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 searchNum;

	class ACameraPlayerController* CameraPlayerController;

	// ����ͼƬ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTexture2D* backgroundImage;

	// ��ʼ��ѡ��ҳ��
	UFUNCTION(BlueprintCallable)
		void initSelect();
	
	// ����ѡ���ص�����
	UFUNCTION(BlueprintCallable)
		void HouseNetCall();

	// ���ѡ���ص�����
	UFUNCTION(BlueprintCallable)
		void DesignNetCall();

	// ����ͼƬ�����¼�
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataBGImage();

	// ������һҳ��ť
	UFUNCTION(BlueprintCallable)
		void btnUp();

	// ������һҳ��ť
	UFUNCTION(BlueprintCallable)
		void btnDown();

	// UI ��ʾ����
	void DisplayConfiguration();

	// �������ύ�¼�
	UFUNCTION(BlueprintCallable)
		void submitSearch(FText submitText);

	// �ı��ı����¼�
	UFUNCTION(BlueprintCallable)
		void changeTextBox(FText btnName);

	// �ı���ı����ص��¼�
	UFUNCTION(BlueprintImplementableEvent)
		void EventchangeTextBox();

	// ����ͼƬ�����¼�
	UFUNCTION(BlueprintImplementableEvent)
		void RefreshPage();

	// ��������������
	UFUNCTION(BlueprintCallable)
		void searchInit();

	// ��������������¼�
	UFUNCTION(BlueprintImplementableEvent)
		void RefreshSearch();

	// �¼� ������ UI_Image ����
	UFUNCTION(BlueprintImplementableEvent)
		void EventArrayUIImage();

	// �ı� CGI ����
	UFUNCTION(BlueprintCallable)
		void ChangeInst();

	// ��ȡ�����б�
	void loadHOUSETYPELISTSchemeDetail();
	void LoadHOUSETYPELISTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void log(FString myString);

	// ��ȡ�����б�
	UFUNCTION(BlueprintCallable)
	void loadSCHEMELISTSchemeDetail(bool bNumPhoto);
	void LoadSCHEMELISTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// �ж��Ƿ�Ϊ�ֻ�����
	UFUNCTION(BlueprintImplementableEvent)
	void EventIsNumPhoto();

	FString getContentWithMap(TMap<FString, FString> params);
};
