// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_mainPanoLoading.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_mainPanoLoading : public UUserWidget
{
	GENERATED_BODY()

public:

	class ACameraPlayerController* CameraPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* backgroundImage;

	// 更新背景图片
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataBGImage();

	// 初始化
	void initMainPanoLoading();
	
	
};
