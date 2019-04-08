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

	// �ı���Ϸʵ������, ��ͼ��ת level ʱ����
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstance();
	
};
