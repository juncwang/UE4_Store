// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CUI_select.h"
#include "UI_index.h"
#include "CUI_loading.h"
#include "CUI_main.h"
#include "CUI_mainPanoLoading.h"
#include "CUI_mainCore.h"
#include "CUI_netError.h"
#include "Store_VRGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API AStore_VRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	// 保存方案
	UFUNCTION(BlueprintCallable)
		void SaveProgram();
	// ===============================================================================

	// 网络错误UI
	UCUI_netError* netErrorUI;

	// 是否为保存退出
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bSaveExit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UCUI_netError> T_netErrorUI;

	UFUNCTION(BlueprintCallable)
		void CreateNetErrorUI();

	UFUNCTION(BlueprintCallable)
		void RemoveNetErrorUI();

	// ===============================================================================

	class ACameraPawn* cameraPawn;

	// 返回全景图二维码查看页面
	UCUI_mainCore* mainCore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UCUI_mainCore> T_mainCore;

	UFUNCTION(BlueprintCallable)
		void mainCoreUI();

	//================================================================================

	// 全景图拍摄中页面
	UCUI_mainPanoLoading* mainPanoLoading;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCUI_mainPanoLoading> T_mainPanoLoading;

	// 删除全景图拍摄背景 UI
	UFUNCTION(BlueprintCallable)
		void removeMainPanoLoading();

	// 手机号码保存位置
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString photoNum;

	UFUNCTION(BlueprintCallable)
		void LoadPanoUI();

	//=============================================================================================================

	// 拍摄全景图的方式( 多张还是单张 )
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAllPano;

	// 主题页面实例
	UCUI_main* ui_main;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCUI_main> Tui_main;

	// 触发材质菜单方法
	UFUNCTION(BlueprintCallable)
		void MaterialMenu();

	// 触发收起所有菜单事件
	UFUNCTION(BlueprintCallable)
		void CloseAllMenu();

	// 打开保存选项
	UFUNCTION(BlueprintCallable)
		void openSaveTitle();

	// 加载页面实例
	UCUI_loading* ui_loading;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCUI_loading> Tui_loading;

	// 选择页面实例
	UCUI_select* ui_select;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCUI_select> Tui_select;

	// 首页页面实例
	UUI_index* ui_index;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUI_index> Tui_index;

	// 获取游戏例子
	class UCameraGameInstance* CGInstance;

	AStore_VRGameModeBase();

	virtual void BeginPlay() override;

	// 获取文件目录
	FString GetFileDir(bool isShipping);

	// 加载首页 UI
	void LoadIndexUI();

	// 加载选择页 UI
	void LoadSelectUI();

	// 加载加载页 UI 
	void LoadLoadingUI();

	// 加载主页 UI
	void LoadMainPageUI();

	// 更新总价及平均价格
	UFUNCTION(BlueprintCallable)
	void UpdataMatinPage(float price);

	// 增加的价格为
	float addPrice;

	// 调用 selectui 的 submitSearch 方法
	UFUNCTION(BlueprintCallable)
		void runSelectUISearch(FText submitText, FText btnName);

	void isShowHouseData();

	UFUNCTION(BlueprintCallable)
		void SetBrandType(int32 num);

	void log(FString mystring);

	// 是否直接退出
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bErrorQuit;

	// 读取品牌列表信息
	// 1：家具 2：地面 3：墙面
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
// 		int32 GameModeBRANDType;
};
