// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CUI_modeSelect.h"
#include "CUI_materialSelect.h"
#include "CameraPawn.generated.h"

UENUM()
enum class EPawnState : uint8
{
	PawnFloorMove,		// 摄像机为地面移动状态
	PawnSkyRotator,		// 摄像机为天空旋转状态
	PawnChanging		// 摄像机状态切换中
};

UCLASS()
class STORE_VR_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:

	// 右键按下松开时间
	float rightMouseTime;

	// 禁用鼠标操作场景
	bool bMouseController;

	// UI 隐藏后不能点击物体
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bMouseUI;

	UFUNCTION(BlueprintCallable)
		void OpenMouseController();

	UFUNCTION(BlueprintCallable)
		void CloseMouseController();

	UCUI_modeSelect* UImodeSelect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UCUI_modeSelect> T_UImodeSelect;

	bool bUImodeSelect;

	UFUNCTION(BlueprintCallable)
		void CreateUImodeSelect();

	UFUNCTION(BlueprintCallable)
		void ShowUImodeSelect();

	UFUNCTION(BlueprintCallable)
		void CloseUImodeSelect();

	UCUI_materialSelect* UImaterialSelect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		TSubclassOf<UCUI_materialSelect> T_UImaterialSelect;

	bool bUImaterialSelect;

	UFUNCTION(BlueprintCallable)
		void CreateUIMaterialSelect();

	UFUNCTION(BlueprintCallable)
		void ShowUIMaterialSelect();

	UFUNCTION(BlueprintCallable)
		void CloseUIMaterialSelect();

	// ==============================================================================================================

	class AStore_VRGameModeBase* gameModeBase;

	class UCameraGameInstance* CGInstance;

	// Pawn 运动组件,添加后 Pawn 才可以移动
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraPawn")
		UPawnMovementComponent* MovementCom;

	// 碰撞组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraPawn")
		class USphereComponent* SphereCom;

	// 摄像机延长线组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraPawn")
		class USpringArmComponent* ArmCom;

	// 摄像机组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraPawn")
		class UCameraComponent* CameraCom;

	// 相机控制器
	class ACameraPlayerController* cameraPlayerController;

	// 定义时间句柄
	FTimerHandle timeHandle;

	

	// 摄像机的操作状态
	EPawnState PawnState;

	// 摄像机的改变后的目标状态
	EPawnState PawnTargetState;

	// 相机当前位置
	FVector ActorLocation;
	// 相机在地面时 Yaw 的旋转值
	float ActorFloorYaw;

	// 天空背景
	TArray<AActor*> SkyActorArr;

	// 地板
	TArray<AActor*> FloorActorArr;

	// 墙面
	TArray<AActor*> WallActorArr;

	// 物体
	TArray<class ABaseMode*> ModeActorArr;

	// 全景图控件
	class APanoActor* PanoActor;

	// 多全景图点
	TArray<class APanoPoint*> PanoPointArr;

	// 鼠标右键是否按下, 控制旋转
	bool bMouseRightDown;

	// 鼠标右键按下到抬起所使用的时间
	float fMouseRightTime;

	// 控制器目标旋转值
	FRotator ControlTargetRotation;

	// 按下鼠标右键时的旋转值
	float RotationDownYaw;
	float RotationDownPitch;

	// 抬起鼠标右键时的旋转值
	float RotationUpYaw;
	float RotationUpPitch;

	// 当前控制的模型, 如果需要失去控制或删除, 请至于 nullprt
	class ABaseMode* PawnBaseMode;

	// 当前选择的墙或地, 如果需要失去控制, 请至于 nullprt
	AActor* WorF_Actor;
	// 当前选择的墙或地, 如果需要失去控制, 请至于 nullprt
	UStaticMeshComponent* WorF_Comp;

	// 是否是鼠标左键选者组件时
	bool bMouseSelectWorF;

	// 是否正在全景图拍摄中
	bool bPanoCap;
	
	// ------------------------------------ RuntimeData/CameraPawnData.json 配置的值 -------------------------------

	// ArmCom 的最小长度
	float ArmComMinLength;

	// ArmCom 的最大长度
	float ArmComMaxLength;

	// 移动最大速度的速率
	float MaxMoveRate;

	// 旋转最大速度的速率
	float MaxRotRate;

	// 旋转 Pitch 轴是否反转
	bool bPitch;

	// 控制器 SKY 状态旋转值
	FRotator SkyControllerRotator;

	// Pawn 状态切换速度
	float PawnChangeSpeed;

	// ArmCom 的长度容错率
	float ArmComFaultTolerance;

	// 是否开启控制选择缓冲
	bool bRotationCase;

	// 控制旋转缓冲减速的倍率
	float RotationalMagnification;

	// 鼠标中建旋转模型的速度
	float PawnBaseModeRotationSpeed;

	// 相机高度
	float CameraHeight;

public:
	// Sets default values for this pawn's properties
	ACameraPawn();

	/*
	* 初始化 CameraPawn 内的数据
	* 读取 RuntimeData/CameraPawnData.json 的值
	* 需要初始化的数据如下:
	* 移动最大速度的速率 MaxMoveRate
	* 旋转最大速度的速率 MaxRotRate
	* 旋转 Pitch 轴是否反转 bPitch
	* ArmCom 的最小长度
	* ArmCom 的最大长度
	* 控制器 SKY 状态旋转值
	* Pawn 状态切换速度
	* ArmCom 长度容错率, 越小越精准, 但是切换时间越慢
	* 控制旋转减速的倍率
	*/
	void InitCameraPawnData();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	// 控制相机前进后退移动
	void MoveForward(float val);

	// 控制相机向右向左移动
	void MoveRight(float val);

	// 鼠标 X 轴控制相机 YAW 轴旋转
	void RotYaw(float val);

	// 鼠标 Y 轴控制相机 PITCH 轴旋转
	void RotPitch(float val);

	// 相机臂初始的长度
	void InitArmLength();

	// 初始化控制器的选择方向
	void InitControlRotation();

	// 始化场景内可用物体
	/************************************************************************/
	/* 场景内的环境雾需删除, 并且天空球背景球的第一个 actor.tag 至为 "skysphere"  */
	/* 场景内的地板第一个 actor.tag 至为 "floor"							*/
	/* 场景内的墙面第一个 actor.tag 至为 "wall"								*/
	/* 场景内的全景相机的 actor.tag 至为 "pano"                             */
	/* 场景内的全景图点的 actor.tag 至为 "panopoint"						*/
	/************************************************************************/
	UFUNCTION(BlueprintCallable)
	void InitSceneActor();

	// --------------------------------------- UI 组件功能 ------------------------------- //

	// 改变 Pawn 状态 视角切换
	void ChangePawnState();

	// 发射一个模型
	// void SpawnNewMode(FString staticMeshPath);   // 加入到 UI 后使用这个后,需传入一个模型文件地址
	UFUNCTION(BlueprintCallable)
	void SpawnNewMode(FString url, FString fileName, int32 gid);

	// 把模型的状态改为可移动状态
	UFUNCTION(BlueprintCallable)
	void PawnBaseModeMove();

	// 删除选中的模型
	UFUNCTION(BlueprintCallable)
	void RemovePawnBaseMode();

	// 更换材质
	// void ChangeNewMateril(FString staticMeshPath);	// 加入到 UI 后使用这个后,需传入一个材质文件地址
	UFUNCTION(BlueprintCallable)
	void ChangeNewMateril(FString url, FString fileName, FName gid);

	// 获取所有的模型
	// 保存方案时需使用 所有模型, 地板, 前面
	void LoadAllBaseActor();

	// 拍摄单张全景图
	void PanoSimpleCap();

	// 拍摄多种全景图
	void PanoAllCap();

	

	// ---------------------------------------------------------------------------------- //

	// 鼠标右建按下方法
	void MouseRightDown();

	// 鼠标右键抬起方法
	void MouseRightUp();

	// 鼠标左键按下方法
	UFUNCTION(BlueprintCallable)
	void MouseLeftDown();

	// 鼠标上滚方法
	void MouseWheelUp();

	// 鼠标下滚方法
	void MouseWheelDown();

	// 从模型类获取模型
	void setBaseMode( class ABaseMode* mode);

	// 地板被点击后触发方法
	UFUNCTION()
		void FloorOnClick(AActor* TouchedActor, FKey ButtonPressed);

	// 墙面被点击后触发方法
	UFUNCTION()
		void WallOnClick(AActor* TouchedActor, FKey ButtonPressed);

	// 在荧屏上打印字符串
	void OutPrint(FString myString);
	
	
	USoundWave* GetSoundWaveFromFile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundWave* sw;

	UFUNCTION(BlueprintImplementableEvent)
		void PlaySoundEvent();
};
