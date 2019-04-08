// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_mainExit.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_mainExit : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void initExitUI();

	class UCameraGameInstance* cameraGameInstance;

	// 改变游戏实例属性, 蓝图跳转 level 时调用
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstance();
	
};
