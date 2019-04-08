// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_search.h"
#include "CUI_select.h"
#include "Http.h"
#include "CUI_Image.generated.h"

// 户型数据结构体
USTRUCT(BlueprintType)
struct FHouseTypeData {

	GENERATED_BODY()

	// 户型名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString HouseName_out;

	// 楼盘名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString PropertyName_out;

	// 户型类型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString HouseType_out;

	// 建筑面积
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Building_out;

	// 套内面积
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Inside_out;

	// 装修风格
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

	// 户型结构体
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FHouseTypeData fHouseData_out;

	// 装载户型结构体回调函数
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
	

	// 结构体对象-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FHouseType HouseType;

	// 是否为 house 类型数据
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHouseType;

	// 设置所有参数值
	UFUNCTION(BlueprintCallable)
		void setProp(FString houseName, FString proName, int32 restNum, int32 rootNum, FString outArea, FString inArea, FString imageURL, EHouseType ehouseType, FString houseGid);

	// 设置风格数据
	UFUNCTION(BlueprintCallable)
		void setPropType(FHouseType fHouseType);

	// 设置GID 数据
	UFUNCTION(BlueprintCallable)
		void setGid(FString inGid, FString inGidScheme, FString houseStyle, FString _phoneNum);
	
	// 改变 UI 显示设置
	UFUNCTION(BlueprintImplementableEvent)
		void EventChange();

	// 设置 CGI 的加载 UI 属性
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstance();

	void loadImageDetail();
	void loadImageCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	bool VerifyOrCreateDirectory(const FString& TestDir);
};
