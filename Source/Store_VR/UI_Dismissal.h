// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Dismissal.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UUI_Dismissal : public UUserWidget
{
	GENERATED_BODY()
	
public:

	class UCameraGameInstance* cameraGameInstance;

	// 文档内容
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FString DocContent;

	// 初始化 Dismissal 的方法
	UFUNCTION(BlueprintCallable)
		void initUIDismissal();

	// 获取免职申明文档( 本地或网络 - 根据最后需求 - 目前为本地获取 ）
	void getDocDismissal();

	// 把文档数据发送给蓝图用于更改组件文字内容
	UFUNCTION(BlueprintImplementableEvent)
		void DismissalToBP();

	// 改变游戏实例属性, 蓝图跳转 level 时调用
	UFUNCTION(BlueprintCallable)
	void ChangeGameInstance();
};
