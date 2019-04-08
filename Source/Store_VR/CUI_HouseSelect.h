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

	// ����gid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString _houseGid;

	// �ṹ�����-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FHouseType HouseType;

	// �ṹ�����-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FHouseTypeData HouseTypeData;

	// ���ýṹ����� FHouseTypeData �е� HouseStyle ����
	UFUNCTION(BlueprintCallable)
		void setHouseStyle(FString _houseStyle);

	// ��ʼ��
	UFUNCTION(BlueprintCallable)
		void initHouseSelect(FHouseType fHouseType, FHouseTypeData fHouseTypeData, FString houseGid);
	
	// ����UI 
	UFUNCTION(BlueprintImplementableEvent)
		void EventUpdataUI();

	// ���� CGI �ļ��� UI ����
	UFUNCTION(BlueprintCallable)
		void ChangeGameInstance();

	void log(FString myString);
};
