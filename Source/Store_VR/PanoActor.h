// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PanoActor.generated.h"

// ������������
USTRUCT(BlueprintType)
struct FOtherArrowData {

	GENERATED_BODY()

	// ����ָ��ķ�������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString arrowName;

	// ����ָ��� Yaw ����ת����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString arrowYaw;

	// ��ǰ��λ������һ��id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString homeGid;

	// ��ǰָ����һ��id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString arrowGid;
};

// ������
USTRUCT(BlueprintType)
struct FPointData {

	GENERATED_BODY()

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString pointName;

	// ����������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FOtherArrowData> otherArrowArray;
};



UCLASS()
class STORE_VR_API APanoActor : public AActor
{
	GENERATED_BODY()

public:
	class ACameraPlayerController* CameraController;
	class ACameraPawn* CameraPawn;
	class AStore_VRGameModeBase* gameMode;
	
	// ���е���ı���Ϣ 
	FString panoPointData;
	// ���е�Ľṹ������
	TArray<FPointData> pointDataArray;

	// ���е������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "prop")
	TArray<FVector> panoPointArr;
	
public:	
	// Sets default values for this actor's properties
	APanoActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// ������ͼ���㵥��ȫ��ͼ����
	UFUNCTION(BlueprintImplementableEvent)
		void PanoSimpleStart();

	// ������ͼ�������ȫ��ͼ����
	UFUNCTION(BlueprintImplementableEvent)
		void PanoAllStart();

	// ��λ�Ƿ��Ѿ�Ϊ��
	UFUNCTION(BlueprintCallable)
		void panoPointArrIsNULL();

	// �ı�������� pawn
	UFUNCTION(BlueprintCallable)
		void ChangeControllerView();
	
};
