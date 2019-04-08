// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CUI_Image.h"
#include "CameraGameInstance.generated.h"

UENUM()
enum class EMaps
{
	Index,		// 首页
	Select,		// 选择户型页
	Loading,	// 资源加载页
	MainPage	// 3D页面
};

/**
 * 
 */
UCLASS()
class STORE_VR_API UCameraGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public: 

	UCameraGameInstance();

	// 户型 gid 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isAdmin;

	// 户型 gid 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString loginPhone;

	// 户型 gid 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString houseGid;

	// 房型 GID 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gid;

	// 房型方案 GID 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gidScheme;

	// 模型 gid 组
	TArray<FString> _modelGidArray;
	// 模型 文件名 组
	TArray<FString> _modelFileNameArray;
	// 模型 文件路径 组
	TArray<FString> _modelFilePathArray;
	// 材质 gid 组
	TArray<FString> _materialGidArray;
	// 材质 文件名 组
	TArray<FString> _materialFileNameArray;
	// 材质 文件路径 组
	TArray<FString> _materialFilePathArray;

	// 结构体对象-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FHouseTypeData HouseTypeData;


	// 地图的配置文件地址 url 或 配置文件
	// 如果 EHouseType 为 House 属性, 那么返回空字符串 "" 
	// 并接受
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString MapsConfiguration;

	// 地图的名称, 蓝图通过名称进行切换 三维地图
	// 如果 EHouseType 为 House 属性, 那么返回空字符串 ""
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString pathMap;

	// 地图在本地的位置, 用于程序检查本地是否有地图文件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString CleckPathMap;

	// 房型对应户型的基础资料
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString HouseDataJson;

	// 当前页面是什么
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EMaps emap;

	// 现代风格数据
	// 地图的配置文件地址 url 或 配置文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XDMapsConfiguration;

	// 地图的名称, 蓝图通过名称进行切换 三维地图
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XDpathMap;

	// 地图在本地的位置, 用于程序检查本地是否有地图文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XDCleckPathMap;

	// 新中式风格数据
	// 地图的配置文件地址 url 或 配置文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XZSMapsConfiguration;

	// 地图的名称, 蓝图通过名称进行切换 三维地图
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XZSpathMap;

	// 地图在本地的位置, 用于程序检查本地是否有地图文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XZSCleckPathMap;

	// 北欧风格数据
	// 地图的配置文件地址 url 或 配置文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BOMapsConfiguration;

	// 地图的名称, 蓝图通过名称进行切换 三维地图
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BOpathMap;

	// 地图在本地的位置, 用于程序检查本地是否有地图文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BOCleckPathMap;

	// 小美风格数据
	// 地图的配置文件地址 url 或 配置文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XMMapsConfiguration;

	// 地图的名称, 蓝图通过名称进行切换 三维地图
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XMpathMap;

	// 地图在本地的位置, 用于程序检查本地是否有地图文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XMCleckPathMap;

	// 东南亚风格数据
	// 地图的配置文件地址 url 或 配置文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString DNYMapsConfiguration;

	// 地图的名称, 蓝图通过名称进行切换 三维地图
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString DNYpathMap;

	// 地图在本地的位置, 用于程序检查本地是否有地图文件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString DNYCleckPathMap;

	void log();
	
// 	USoundWave* GetSoundWaveFromFile();
// 
// 	void playSound();
};
