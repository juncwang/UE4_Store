// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_mainList.generated.h"


// ��������������
USTRUCT(BlueprintType)
struct FListData
{
	GENERATED_BODY()

	// ��Ʒ gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString gid;

	// ��Ʒ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString productName;

	// ��Ʒ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString productTypeName;

	// ��Ʒ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString productCode;

	// ��Ʒ��ע
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString productMarkers;

	// ��Ʒ�۸�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float unitPrice;

	// ��Ʒ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float number;
};
/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_mainList : public UUserWidget
{
	GENERATED_BODY()
	
public:

	class ACameraPawn* cameraPawn;
	class AStore_VRGameModeBase* gameMode;

	// gid �б�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> gidArray;

	// �����б�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FListData> listDataArray;

	// ��Ʒ����
	// 0 Ϊģ��, 1 Ϊ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int32> CPType;

	// �ܼ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float TotalPrice;

	// ��ʼ����
	UFUNCTION(BlueprintCallable)
		void initMainList();

	// ��ʼ����ͼ�¼�
	UFUNCTION(BlueprintImplementableEvent)
		void initBPMainList();
	
	// ��ȡ�б�۸�����
	void loadPRICESchemeDetail();
	void LoadPRICESchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void log(FString myString);
};
