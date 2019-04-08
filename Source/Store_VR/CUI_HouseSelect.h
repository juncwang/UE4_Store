// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_select.h"
#include "CUI_Image.h"
#include "CUI_HouseSelect.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_HouseSelect : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// 户型gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString _houseGid;

	// 结构体对象-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FHouseType HouseType;

	// 结构体对象-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FHouseTypeData HouseTypeData;

	// 设置结构体对象 FHouseTypeData 中的 HouseStyle 参数
	UFUNCTION(BlueprintCallable)
		void setHouseStyle(FString _houseStyle);

	// 初始化
	UFUNCTION(BlueprintCallable)
		void initHouseSelect(FHouseType fHouseType, FHouseTypeData fHouseTypeData, FString houseGid);
	
	// 更新UI 
	UFUNCTION(BlueprintImplementableEvent)
		void EventUpdataUI();

	// 设置 CGI 的加载 UI 属性
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstance();

	void log(FString myString);
};
