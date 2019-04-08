// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "UI_index.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UUI_index : public UUserWidget
{
	GENERATED_BODY()
	
public:

	class ACameraPlayerController* CameraPlayerController;
	class AStore_VRGameModeBase* gameMode;
	class UCameraGameInstance* cgi;

	// �汾�� ���ذ汾�� ����汾��
	FString localVersion;
	FString netVersion;

	// ���ص�ַ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString downloadLink;

	// ����ͼƬ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* backgroundImage;

	// ��ʼ�� UI_index ���
	UFUNCTION(BlueprintCallable)
		void initIndex();

	// ����Ƿ����°汾
	void checkVersion();

	// �ȶ԰汾��
	void ComparedVersion(FString version);

	// ���������¼�
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataVersion();

	// ����ͼƬ�����¼�
	UFUNCTION(BlueprintImplementableEvent)
		void UpdataBGImage();

	// ��ȡ����汾��Ϣ
	void loadVersionSchemeDetail();
	void LoadVersionSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	void log(FString myString);
	
};
