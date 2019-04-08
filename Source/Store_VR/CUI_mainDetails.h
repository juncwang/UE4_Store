// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_mainDetails.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_mainDetails : public UUserWidget
{
	GENERATED_BODY()
	
public:

	class ACameraPawn* cameraPawn;
	class AStore_VRGameModeBase* gameMode;
	class ACameraPlayerController* cameraController;

	// ��Ʒ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UnitCode;

	// ��Ʒ���ӵ�ַ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UnitURI;

	// ��Ʒ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UnitPrice;

	// Ʒ�Ƽ���Ʒ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BrandAndProductName;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString StyleName;

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString MaterialName;

	// ��Ʒ�ߴ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString SizeName;

	// ��Ʒ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BearingName;

	// �̼�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UserName;

	// �̼ҵ�ַ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString UserAddress;

	// ͼƬ���ص�ַ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString ImageUrl;

	// ͼƬ����·��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString ImagePath;

	// ���� UI �¼�
	UFUNCTION(BlueprintImplementableEvent)
		void EventChangeUI();

	// ͼƬ�ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTexture2D* ImageTexture;

	// ���� ͼƬ �¼�
	UFUNCTION(BlueprintImplementableEvent)
		void EventChangeImage();

	// �ر� �¼�
	UFUNCTION(BlueprintImplementableEvent)
		void EventClose();

	FString gid;

	// �ص�����, ��������
	UFUNCTION(BlueprintCallable)
		void initDetails();

	// ������Ʒ��������
	void loadPRODUCTSchemeDetail();
	void LoadPRODUCTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	// ͼƬ��ȡ
	void loadImageDetail();
	void loadImageCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	bool VerifyOrCreateDirectory(const FString& TestDir);
	
	void log(FString myString);
};
