// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_search.h"
#include "CUI_select.h"
#include "Http.h"
#include "CUI_Image.generated.h"

// �������ݽṹ��
USTRUCT(BlueprintType)
struct FHouseTypeData {

	GENERATED_BODY()

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString HouseName_out;

	// ¥������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString PropertyName_out;

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString HouseType_out;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Building_out;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Inside_out;

	// װ�޷��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString HouseStyle;

};

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_Image : public UUserWidget
{
	GENERATED_BODY()
	
public:

	class ACameraPlayerController* cameraController;

	// ���ͽṹ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FHouseTypeData fHouseData_out;

	// װ�ػ��ͽṹ��ص�����
	UFUNCTION(BlueprintCallable)
	void InitHouseData(FString houseType_in);

	class AStore_VRGameModeBase* CameraGameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString _houseGid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString pathImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString codeTile_C;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString  Community_C;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32  RestNum_C;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32  RootNum_C;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString  HouseOutArea_C;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString  HouseInArea_C;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UTexture2D*  texture2d;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gid;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gidScheme;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString phoneNum;
	

	// �ṹ�����-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FHouseType HouseType;

	// �Ƿ�Ϊ house ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHouseType;

	// �������в���ֵ
	UFUNCTION(BlueprintCallable)
		void setProp(FString houseName, FString proName, int32 restNum, int32 rootNum, FString outArea, FString inArea, FString imageURL, EHouseType ehouseType, FString houseGid);

	// ���÷������
	UFUNCTION(BlueprintCallable)
		void setPropType(FHouseType fHouseType);

	// ����GID ����
	UFUNCTION(BlueprintCallable)
		void setGid(FString inGid, FString inGidScheme, FString houseStyle, FString _phoneNum);
	
	// �ı� UI ��ʾ����
	UFUNCTION(BlueprintImplementableEvent)
		void EventChange();

	// ���� CGI �ļ��� UI ����
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstance();

	void loadImageDetail();
	void loadImageCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	bool VerifyOrCreateDirectory(const FString& TestDir);
};
