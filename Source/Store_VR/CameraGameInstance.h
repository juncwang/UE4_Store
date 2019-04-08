// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CUI_Image.h"
#include "CameraGameInstance.generated.h"

UENUM()
enum class EMaps
{
	Index,		// ��ҳ
	Select,		// ѡ����ҳ
	Loading,	// ��Դ����ҳ
	MainPage	// 3Dҳ��
};

/**
 * 
 */
UCLASS()
class STORE_VR_API UCameraGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public: 

	UCameraGameInstance();

	// ���� gid 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isAdmin;

	// ���� gid 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString loginPhone;

	// ���� gid 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString houseGid;

	// ���� GID 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gid;

	// ���ͷ��� GID 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString gidScheme;

	// ģ�� gid ��
	TArray<FString> _modelGidArray;
	// ģ�� �ļ��� ��
	TArray<FString> _modelFileNameArray;
	// ģ�� �ļ�·�� ��
	TArray<FString> _modelFilePathArray;
	// ���� gid ��
	TArray<FString> _materialGidArray;
	// ���� �ļ��� ��
	TArray<FString> _materialFileNameArray;
	// ���� �ļ�·�� ��
	TArray<FString> _materialFilePathArray;

	// �ṹ�����-----------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FHouseTypeData HouseTypeData;


	// ��ͼ�������ļ���ַ url �� �����ļ�
	// ��� EHouseType Ϊ House ����, ��ô���ؿ��ַ��� "" 
	// ������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString MapsConfiguration;

	// ��ͼ������, ��ͼͨ�����ƽ����л� ��ά��ͼ
	// ��� EHouseType Ϊ House ����, ��ô���ؿ��ַ��� ""
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString pathMap;

	// ��ͼ�ڱ��ص�λ��, ���ڳ����鱾���Ƿ��е�ͼ�ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString CleckPathMap;

	// ���Ͷ�Ӧ���͵Ļ�������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString HouseDataJson;

	// ��ǰҳ����ʲô
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EMaps emap;

	// �ִ��������
	// ��ͼ�������ļ���ַ url �� �����ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XDMapsConfiguration;

	// ��ͼ������, ��ͼͨ�����ƽ����л� ��ά��ͼ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XDpathMap;

	// ��ͼ�ڱ��ص�λ��, ���ڳ����鱾���Ƿ��е�ͼ�ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XDCleckPathMap;

	// ����ʽ�������
	// ��ͼ�������ļ���ַ url �� �����ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XZSMapsConfiguration;

	// ��ͼ������, ��ͼͨ�����ƽ����л� ��ά��ͼ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XZSpathMap;

	// ��ͼ�ڱ��ص�λ��, ���ڳ����鱾���Ƿ��е�ͼ�ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XZSCleckPathMap;

	// ��ŷ�������
	// ��ͼ�������ļ���ַ url �� �����ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BOMapsConfiguration;

	// ��ͼ������, ��ͼͨ�����ƽ����л� ��ά��ͼ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BOpathMap;

	// ��ͼ�ڱ��ص�λ��, ���ڳ����鱾���Ƿ��е�ͼ�ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString BOCleckPathMap;

	// С���������
	// ��ͼ�������ļ���ַ url �� �����ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XMMapsConfiguration;

	// ��ͼ������, ��ͼͨ�����ƽ����л� ��ά��ͼ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XMpathMap;

	// ��ͼ�ڱ��ص�λ��, ���ڳ����鱾���Ƿ��е�ͼ�ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString XMCleckPathMap;

	// �����Ƿ������
	// ��ͼ�������ļ���ַ url �� �����ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString DNYMapsConfiguration;

	// ��ͼ������, ��ͼͨ�����ƽ����л� ��ά��ͼ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString DNYpathMap;

	// ��ͼ�ڱ��ص�λ��, ���ڳ����鱾���Ƿ��е�ͼ�ļ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString DNYCleckPathMap;

	void log();
	
// 	USoundWave* GetSoundWaveFromFile();
// 
// 	void playSound();
};
