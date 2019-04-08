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

	// 蓝图初始化
	UFUNCTION(BlueprintImplementableEvent)
		void mainInitLoading();

	// 蓝图初始化
	UFUNCTION(BlueprintImplementableEvent)
		void mainCloseLoading();

	// 相机角色
	class ACameraPawn* cameraPawn;
	class UCameraGameInstance* cameraInstance;
	class AStore_VRGameModeBase* gameMode;

	// 初始化 UI
	UFUNCTION(BlueprintCallable)
	void initMain();

	// 蓝图初始化
	UFUNCTION(BlueprintImplementableEvent)
		void initMainBP();

	// 切换视角按钮回调
	UFUNCTION(BlueprintCallable)
		void ViewChange();

	// 切换视角按钮回调
	UFUNCTION(BlueprintCallable)
		void ViewFirstChange();

	// 是否显示户型数据事件
	UFUNCTION(BlueprintImplementableEvent)
		void isShowHouseData();

	// 更新 UI 上户型的基础资料
	UFUNCTION(BlueprintImplementableEvent)
		void EventUpdataHouseBase();

	// 更新户型风格按钮事件
	UFUNCTION(BlueprintImplementableEvent)
		void EventUpdataStyleBtn();

	// 改变 maps 为 loading 页面
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstanceToLoading();

	// 开始拍摄单张全景图
	UFUNCTION(BlueprintCallable)
		void PanoSimpleStart();

	// 开始拍摄多张全景图
	UFUNCTION(BlueprintCallable)
		void PanoAllStart();

	// 打开添加商品按钮后回调函数
	UFUNCTION(BlueprintCallable)
		void AddMenuDown();

	// 打开保存选项
	UFUNCTION(BlueprintImplementableEvent)
		void openSaveTitle();

	// 触发材质菜单事件
	UFUNCTION(BlueprintImplementableEvent)
		void MaterialMenu();

	// 触发收起所有菜单事件
	UFUNCTION(BlueprintImplementableEvent)
		void CloseAllMenu();

	void log(FString myString);

	// 网络接口访问方法

	// 品牌目前选择
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 BrandSelectGid;
	// 风格目前选择
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 StyleSelectGid;
	// 类型目前选择
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 TypeSelectGid;

	// 读取品牌列表信息
	// 1：家具 2：地面 3：墙面
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 BRANDType;	// 品牌, 类型公用属性
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

	// 读取风格列表信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> STYLEName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> STYLEGid;
	void loadSTYLESchemeDetail();
	void LoadSTYLESchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataStyle();

	// 读取商品类型列表信息
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> TYPEName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> TYPEGid;
	void loadTYPESchemeDetail();
	void LoadTYPESchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataType();


	// 商品列表
	// 当前页数
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 curPage;
	// 请求回最大数据总量
	int32 pageSize;
	// 图片地址组
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> imgURLArray;
	// 图片地址组
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> _fileIdArray;
	// 文件路径
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> FilePathArray;
	// 文件名称
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> FileNameArray;
	// 下载地址
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> DownPathArray;
	// 文件 gid
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<int32> gidArray;
	// 上一页按钮
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isPrevPage;
	// 下一页按钮
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isNextPage;
	// 更新商品列表
	UFUNCTION(BlueprintCallable)
		void UpdataCommodity();
	UFUNCTION(BlueprintImplementableEvent)
		void UpadataComList();
	void loadCommoditySchemeDetail();
	void LoadCommoditySchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/************************************************************************/
	/* 初始化 页面数据                                                      */
	/************************************************************************/

	void initMainData();

	// 户型详情获取
	void loadHOUSEINFOSchemeDetail();
	void LoadHOUSEINFOSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 区域名称组
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> areaNameArray;
	// 区域大小组
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FString> areaSizeArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString deliveryArea;

	UFUNCTION(BlueprintImplementableEvent)
		void UpdataHouseInfo();

	// 获取风格
	void loadROOMLISTSchemeDetail();
	void LoadROOMLISTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 结构体对象-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FHouseType HouseType;


	// 更新价格
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
