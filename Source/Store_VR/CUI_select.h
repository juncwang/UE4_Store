// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_select.generated.h"

// 图片显示框数据类型
UENUM()
enum class EHouseType :uint8
{
	House,		// 户型数据
	Design,		// 设计数据
};

// 搜索框数据类型
USTRUCT(BlueprintType)
struct FSearchData
{
	GENERATED_BODY()

	// 按钮上面显示的内容, 点击后通过该内容进行搜索
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString searchBtn;

	// 点击按钮后请求的 url 地址
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString url;
};

// 户型风格地图
// 如果户型没有下面的属性, 那么返回字符串 ""
USTRUCT(BlueprintType)
struct FHouseType
{
	GENERATED_BODY()

	// 现代风格数据 gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XDGid;

	// 方案 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XDSchemeGid;

	// 现代风格数据风格 id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XDConfigStyleGid;

	// 新中式风格数据 gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XZSGid;

	// 方案 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XZSSchemeGid;

	// 新中式风格数据风格 id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XZSConfigStyleGid;

	// 北欧风格数据 gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 BOGid;

	// 方案 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 BOSchemeGid;

	// 北欧风格数据风格 id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 BOConfigStyleGid;

	// 小美风格数据 gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XMGid;

	// 方案 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XMSchemeGid;

	// 小美风格数据风格 id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 XMConfigStyleGid;

	// 东南亚风格数据 gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 DNYGid;

	// 方案 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 DNYSchemeGid;

	// 东南亚风格数据风格 id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 DNYConfigStyleGid;

};

// 展示框数据类型
USTRUCT(BlueprintType)
struct FHouseData
{
	GENERATED_BODY()

	// 地图的配置文件地址 url 或 配置文件
	// 如果 EHouseType 为 House 属性, 那么返回空字符串 "" 
	// 并接受
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString MapsConfiguration;

	// 地图的名称, 蓝图通过名称进行切换 三维地图
	// 如果 EHouseType 为 House 属性, 那么返回空字符串 ""
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString pathMap;

	// 地图在本地的位置, 用于程序检查本地是否有地图文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString CleckPathMap;

	// map 的背景图片
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString pathImage;

	// 户型名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString codeTile;

	// 所属楼盘
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString Community;

	// 户型类型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString HouseType;

	// 建筑面积
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString HouseOutArea;

	// 套内面积
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

	// 从网络取回的搜索框的值
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FSearchData> searchDataArray;

	// 从网络获取的房屋数据
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FHouseData> houseDataArray;

	// 户型 gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> HouseGidArray;

	// 房间名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> HouseNameArray;

	// 楼盘名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> PropertyNameArray;

	// 房间餐厅数量
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int32> RestaurantNumArray;

	// 房间卧室数量
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int32> RoomNumArray;

	// 建筑面积
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> OutAreaArray;

	// 套内面积
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> InAreaArray;

	// 图片地址
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> ImageUrlArray;

	// 风格类型数据
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FHouseType> houseTypeArray;

	// 当状态为 Design 时 需要获取的数据===========================================================================================

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	// 房型 gid 数据类型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> gidArray;

	// 房型 gid 数据类型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> phoneNumArray;

	// 房型方案 gid 数据类型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> gidSchemeArray;

	// 风格名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FString> HouseStyleArray;

	// ===============================================================================================================================

	// 文本框内容
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FText textName;

	// 当前页数
	int32 pageNum;

	// 数据总量
	int32 dataMax;

	// 最大页数
	int32 pageMax;

	// 需要哪种数据
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EHouseType eHouse;

	// 那种搜索属性
	bool bSearch;

	// 搜索数据是否为空
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isSearchData;

	// 当前页面数据数量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 pageNowNum;

	// 是否显示向前按钮
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isShowUpBtn;

	// 是否显示向后按钮
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isShowNetBtn;

	// 搜索关键字
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FString searchKeyword;

	// 热门搜索数量
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int32 searchNum;

	class ACameraPlayerController* CameraPlayerController;

	// 背景图片
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTexture2D* backgroundImage;

	// 初始化选择页面
	UFUNCTION(BlueprintCallable)
		void initSelect();
	
	// 户型选择后回调函数
	UFUNCTION(BlueprintCallable)
		void HouseNetCall();

	// 设计选择后回调函数
	UFUNCTION(BlueprintCallable)
		void DesignNetCall();

	// 触发图片加载事件
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataBGImage();

	// 按下上一页按钮
	UFUNCTION(BlueprintCallable)
		void btnUp();

	// 按下下一页按钮
	UFUNCTION(BlueprintCallable)
		void btnDown();

	// UI 显示配置
	void DisplayConfiguration();

	// 搜索框提交事件
	UFUNCTION(BlueprintCallable)
		void submitSearch(FText submitText);

	// 改变文本框事件
	UFUNCTION(BlueprintCallable)
		void changeTextBox(FText btnName);

	// 改变后文本框后回调事件
	UFUNCTION(BlueprintImplementableEvent)
		void EventchangeTextBox();

	// 触发图片加载事件
	UFUNCTION(BlueprintImplementableEvent)
		void RefreshPage();

	// 配置热门搜索框
	UFUNCTION(BlueprintCallable)
		void searchInit();

	// 触发搜索框加载事件
	UFUNCTION(BlueprintImplementableEvent)
		void RefreshSearch();

	// 事件 把所有 UI_Image 打组
	UFUNCTION(BlueprintImplementableEvent)
		void EventArrayUIImage();

	// 改变 CGI 属性
	UFUNCTION(BlueprintCallable)
		void ChangeInst();

	// 获取户型列表
	void loadHOUSETYPELISTSchemeDetail();
	void LoadHOUSETYPELISTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void log(FString myString);

	// 获取方案列表
	UFUNCTION(BlueprintCallable)
	void loadSCHEMELISTSchemeDetail(bool bNumPhoto);
	void LoadSCHEMELISTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 判断是否为手机号码
	UFUNCTION(BlueprintImplementableEvent)
	void EventIsNumPhoto();

	FString getContentWithMap(TMap<FString, FString> params);
};
