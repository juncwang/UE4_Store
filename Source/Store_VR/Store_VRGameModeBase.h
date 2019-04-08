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

	// ���淽��
	UFUNCTION(BlueprintCallable)
		void SaveProgram();
	// ===============================================================================

	// �������UI
	UCUI_netError* netErrorUI;

	// �Ƿ�Ϊ�����˳�
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

	// ����ȫ��ͼ��ά��鿴ҳ��
	UCUI_mainCore* mainCore;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UCUI_mainCore> T_mainCore;

	UFUNCTION(BlueprintCallable)
		void mainCoreUI();

	//================================================================================

	// ȫ��ͼ������ҳ��
	UCUI_mainPanoLoading* mainPanoLoading;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCUI_mainPanoLoading> T_mainPanoLoading;

	// ɾ��ȫ��ͼ���㱳�� UI
	UFUNCTION(BlueprintCallable)
		void removeMainPanoLoading();

	// �ֻ����뱣��λ��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString photoNum;

	UFUNCTION(BlueprintCallable)
		void LoadPanoUI();

	//=============================================================================================================

	// ����ȫ��ͼ�ķ�ʽ( ���Ż��ǵ��� )
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAllPano;

	// ����ҳ��ʵ��
	UCUI_main* ui_main;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCUI_main> Tui_main;

	// �������ʲ˵�����
	UFUNCTION(BlueprintCallable)
		void MaterialMenu();

	// �����������в˵��¼�
	UFUNCTION(BlueprintCallable)
		void CloseAllMenu();

	// �򿪱���ѡ��
	UFUNCTION(BlueprintCallable)
		void openSaveTitle();

	// ����ҳ��ʵ��
	UCUI_loading* ui_loading;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCUI_loading> Tui_loading;

	// ѡ��ҳ��ʵ��
	UCUI_select* ui_select;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCUI_select> Tui_select;

	// ��ҳҳ��ʵ��
	UUI_index* ui_index;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUI_index> Tui_index;

	// ��ȡ��Ϸ����
	class UCameraGameInstance* CGInstance;

	AStore_VRGameModeBase();

	virtual void BeginPlay() override;

	// ��ȡ�ļ�Ŀ¼
	FString GetFileDir(bool isShipping);

	// ������ҳ UI
	void LoadIndexUI();

	// ����ѡ��ҳ UI
	void LoadSelectUI();

	// ���ؼ���ҳ UI 
	void LoadLoadingUI();

	// ������ҳ UI
	void LoadMainPageUI();

	// �����ܼۼ�ƽ���۸�
	UFUNCTION(BlueprintCallable)
	void UpdataMatinPage(float price);

	// ���ӵļ۸�Ϊ
	float addPrice;

	// ���� selectui �� submitSearch ����
	UFUNCTION(BlueprintCallable)
		void runSelectUISearch(FText submitText, FText btnName);

	void isShowHouseData();

	UFUNCTION(BlueprintCallable)
		void SetBrandType(int32 num);

	void log(FString mystring);

	// �Ƿ�ֱ���˳�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bErrorQuit;

	// ��ȡƷ���б���Ϣ
	// 1���Ҿ� 2������ 3��ǽ��
// 	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
// 		int32 GameModeBRANDType;
};
