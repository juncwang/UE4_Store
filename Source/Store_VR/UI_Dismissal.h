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

	// �ĵ�����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FString DocContent;

	// ��ʼ�� Dismissal �ķ���
	UFUNCTION(BlueprintCallable)
		void initUIDismissal();

	// ��ȡ��ְ�����ĵ�( ���ػ����� - ����������� - ĿǰΪ���ػ�ȡ ��
	void getDocDismissal();

	// ���ĵ����ݷ��͸���ͼ���ڸ��������������
	UFUNCTION(BlueprintImplementableEvent)
		void DismissalToBP();

	// �ı���Ϸʵ������, ��ͼ��ת level ʱ����
	UFUNCTION(BlueprintCallable)
	void ChangeGameInstance();
};
