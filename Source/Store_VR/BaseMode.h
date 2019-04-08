// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMode.generated.h"

UENUM()
enum class EBaseModeState: uint8
{
	BaseModeMove,	// 移动状态
	BaseModeSelect,	// 选中状态
	BaseModeStatic,	// 静止状态
};

UCLASS()
class STORE_VR_API ABaseMode : public AActor
{
	GENERATED_BODY()

public:

	FCollisionQueryParams params;
	class ACameraPlayerController* CameraPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseActor")
		int32 gid;

	// 场景组件, 负责该类在场景中的位置关系
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseActor")
		USceneComponent* SceneComp;

	// 模型组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseActor")
		UStaticMeshComponent* StaticMeshComp;

	// 模型状态
	EBaseModeState BaseModeState;

	// 是否是鼠标左键选者模型时
	bool bMouseSelect;
	
public:	
	// Sets default values for this actor's properties
	ABaseMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 鼠标左键单击事件
	UFUNCTION()
	void actorOnClicked(AActor* TouchedActor, FKey ButtonPressed);

	// 初始化模型数据
	void initStaticMesh(UStaticMesh *mesh, int32 id);

	// 设置选中状态下显示
	void setRender(bool isShow);

	// 在场景中删除自己
	void destroySelf();

	// 荧屏打印
	void OutPrint(FString myString);
};
