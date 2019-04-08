// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_mainList.generated.h"


// 搜索框数据类型
USTRUCT(BlueprintType)
struct FListData
{
	GENERATED_BODY()

	// 商品 gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString gid;

	// 商品名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString productName;

	// 商品类型
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString productTypeName;

	// 商品编码
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString productCode;

	// 商品备注
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString productMarkers;

	// 商品价格
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float unitPrice;

	// 商品数量
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

	// gid 列表
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> gidArray;

	// 数据列表
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FListData> listDataArray;

	// 商品类型
	// 0 为模型, 1 为材质
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<int32> CPType;

	// 总价
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float TotalPrice;

	// 初始化类
	UFUNCTION(BlueprintCallable)
		void initMainList();

	// 初始化蓝图事件
	UFUNCTION(BlueprintImplementableEvent)
		void initBPMainList();
	
	// 获取列表价格数据
	void loadPRICESchemeDetail();
	void LoadPRICESchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void log(FString myString);
};
