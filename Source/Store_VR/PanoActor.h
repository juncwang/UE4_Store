// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PanoActor.generated.h"

// 其他方向数据
USTRUCT(BlueprintType)
struct FOtherArrowData {

	GENERATED_BODY()

	// 方向指向的房间名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString arrowName;

	// 方向指向的 Yaw 轴旋转度数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString arrowYaw;

	// 当前点位属于哪一个id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString homeGid;

	// 当前指向哪一个id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString arrowGid;
};

// 点数据
USTRUCT(BlueprintType)
struct FPointData {

	GENERATED_BODY()

	// 点的名称
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString pointName;

	// 方向数据组
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
	
	// 所有点的文本信息 
	FString panoPointData;
	// 所有点的结构体数据
	TArray<FPointData> pointDataArray;

	// 所有点的数组
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "prop")
	TArray<FVector> panoPointArr;
	
public:	
	// Sets default values for this actor's properties
	APanoActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// 驱动蓝图拍摄单张全景图方法
	UFUNCTION(BlueprintImplementableEvent)
		void PanoSimpleStart();

	// 驱动蓝图拍摄多张全景图方法
	UFUNCTION(BlueprintImplementableEvent)
		void PanoAllStart();

	// 点位是否已经为空
	UFUNCTION(BlueprintCallable)
		void panoPointArrIsNULL();

	// 改变摄像机到 pawn
	UFUNCTION(BlueprintCallable)
		void ChangeControllerView();
	
};
