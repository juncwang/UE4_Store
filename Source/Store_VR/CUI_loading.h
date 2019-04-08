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

	// 控制器
	class ACameraPlayerController* CameraPlayerController;

	// 游戏句柄
	class UCameraGameInstance* cameraGameInstance;

	// 游戏模式
	class AStore_VRGameModeBase* gameMode;

	// 每个模型的进度值
	float simModelNum;

	// 总进度大小
	int32 numSize;

	// 下载模型时, 上一次加了多少
	float prevNum;

	// 读取的当前进度
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float numLoading;

	// 进度文字
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString textLoading;

	// 背景图片
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTexture2D* backgroundImage;

	// 房型 GID 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gid;

	// 房型方案 GID 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gidScheme;

	// 初始化 UI 界面
	UFUNCTION(BlueprintCallable)
		void initLoading();
	
	// 改变进度条事件
	UFUNCTION(BlueprintImplementableEvent)
		void EventChange();
	
	// 改变背景条事件
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataBG();

	// 查看文件是否存在
	bool isVerifyFile(const FString& Path);

	// 设置 CGI 的加载 UI 属性
	UFUNCTION(BlueprintCallable)
	void ChangeGameInstance();

	void log(FString myString);

	/************************************************************************/
	/* 网络部分                                                             */
	/************************************************************************/

	// 是否下载 json 文件
	bool isJsonFile;
	// 是否下载 map 文件
	bool isMapFile;

	// 文件名
	FString _FileName;
	// 文件路径
	FString _FilePath;
	// 文件下载地址
	FString _FileDownUrl;
	// 文件版本号
	FString _FileId;

	// map 文件名
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString _mapFileName;
	// map 文件路径
	FString _mapFilePath;
	// map 文件下载地址
	FString _mapFileDownUrl;

	// 模型 gid 组
	TArray<FString> _modelGidArray;
	// 模型 文件名 组
	TArray<FString> _modelFileNameArray;
	// 模型 文件路径 组
	TArray<FString> _modelFilePathArray;
	// 模型 文件下载地址 组
	TArray<FString> _modelFileDownUrlArray;
	// 模型版本号组
	TArray<FString> _modelFileIDArray;

	// 材质 gid 组
	TArray<FString> _materialGidArray;
	// 材质 文件名 组
	TArray<FString> _materialFileNameArray;
	// 材质 文件路径 组
	TArray<FString> _materialFilePathArray;
	// 材质 文件下载地址 组
	TArray<FString> _materialFileDownUrlArray;
	// 材质版本号组
	TArray<FString> _materialFileIDArray;

	// 获取房型方案json配置文件
	void GetSchemeJson();
	void loadSCHEMEINFODetail();
	void LoadSCHEMEINFODetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 分配每个物品下载占比
	void CalculatedRatio();

	// 获取 model 内的数据
	void StartModeData();

	// 获取 model 内的模型数据
	void loadMODELINFDetail(FString gidArray1, FString gidArray2);
	void LoadMODELINFDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 获取 material 内的数据
	void StartMaterialData();

	// 获取 material 内的模型数据
	void loadMATERIALINFODetail(FString gidArray);
	void LoadMATERIALINFODetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 检测文件是否存在
	void CleckFile();

	// 获取房型资料
	void loadROOMTYPEDetail();
	void LoadROOMTYPEDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// 网络下载文件方法
	void downloadMesh(FString url);
	void downloadMeshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void downloadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived);

	// 保存文件到本地方法
	bool VerifyOrCreateDirectory(const FString& TestDir);

	// 打开场景
	UFUNCTION(BlueprintImplementableEvent)
	void openMap();
};
