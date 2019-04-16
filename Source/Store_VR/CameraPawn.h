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
	PawnFloorMove,		// �����Ϊ�����ƶ�״̬
	PawnSkyRotator,		// �����Ϊ�����ת״̬
	PawnChanging		// �����״̬�л���
};

UCLASS()
class STORE_VR_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:

	// �Ҽ������ɿ�ʱ��
	float rightMouseTime;

	// ��������������
	bool bMouseController;

	// UI ���غ��ܵ������
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

	// Pawn �˶����,��Ӻ� Pawn �ſ����ƶ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraPawn")
		UPawnMovementComponent* MovementCom;

	// ��ײ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraPawn")
		class USphereComponent* SphereCom;

	// ������ӳ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraPawn")
		class USpringArmComponent* ArmCom;

	// ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraPawn")
		class UCameraComponent* CameraCom;

	// ���������
	class ACameraPlayerController* cameraPlayerController;

	// ����ʱ����
	FTimerHandle timeHandle;

	

	// ������Ĳ���״̬
	EPawnState PawnState;

	// ������ĸı���Ŀ��״̬
	EPawnState PawnTargetState;

	// �����ǰλ��
	FVector ActorLocation;
	// ����ڵ���ʱ Yaw ����תֵ
	float ActorFloorYaw;

	// ��ձ���
	TArray<AActor*> SkyActorArr;

	// �ذ�
	TArray<AActor*> FloorActorArr;

	// ǽ��
	TArray<AActor*> WallActorArr;

	// ����
	TArray<class ABaseMode*> ModeActorArr;

	// ȫ��ͼ�ؼ�
	class APanoActor* PanoActor;

	// ��ȫ��ͼ��
	TArray<class APanoPoint*> PanoPointArr;

	// ����Ҽ��Ƿ���, ������ת
	bool bMouseRightDown;

	// ����Ҽ����µ�̧����ʹ�õ�ʱ��
	float fMouseRightTime;

	// ������Ŀ����תֵ
	FRotator ControlTargetRotation;

	// ��������Ҽ�ʱ����תֵ
	float RotationDownYaw;
	float RotationDownPitch;

	// ̧������Ҽ�ʱ����תֵ
	float RotationUpYaw;
	float RotationUpPitch;

	// ��ǰ���Ƶ�ģ��, �����Ҫʧȥ���ƻ�ɾ��, ������ nullprt
	class ABaseMode* PawnBaseMode;

	// ��ǰѡ���ǽ���, �����Ҫʧȥ����, ������ nullprt
	AActor* WorF_Actor;
	// ��ǰѡ���ǽ���, �����Ҫʧȥ����, ������ nullprt
	UStaticMeshComponent* WorF_Comp;

	// �Ƿ���������ѡ�����ʱ
	bool bMouseSelectWorF;

	// �Ƿ�����ȫ��ͼ������
	bool bPanoCap;
	
	// ------------------------------------ RuntimeData/CameraPawnData.json ���õ�ֵ -------------------------------

	// ArmCom ����С����
	float ArmComMinLength;

	// ArmCom ����󳤶�
	float ArmComMaxLength;

	// �ƶ�����ٶȵ�����
	float MaxMoveRate;

	// ��ת����ٶȵ�����
	float MaxRotRate;

	// ��ת Pitch ���Ƿ�ת
	bool bPitch;

	// ������ SKY ״̬��תֵ
	FRotator SkyControllerRotator;

	// Pawn ״̬�л��ٶ�
	float PawnChangeSpeed;

	// ArmCom �ĳ����ݴ���
	float ArmComFaultTolerance;

	// �Ƿ�������ѡ�񻺳�
	bool bRotationCase;

	// ������ת������ٵı���
	float RotationalMagnification;

	// ����н���תģ�͵��ٶ�
	float PawnBaseModeRotationSpeed;

	// ����߶�
	float CameraHeight;

public:
	// Sets default values for this pawn's properties
	ACameraPawn();

	/*
	* ��ʼ�� CameraPawn �ڵ�����
	* ��ȡ RuntimeData/CameraPawnData.json ��ֵ
	* ��Ҫ��ʼ������������:
	* �ƶ�����ٶȵ����� MaxMoveRate
	* ��ת����ٶȵ����� MaxRotRate
	* ��ת Pitch ���Ƿ�ת bPitch
	* ArmCom ����С����
	* ArmCom ����󳤶�
	* ������ SKY ״̬��תֵ
	* Pawn ״̬�л��ٶ�
	* ArmCom �����ݴ���, ԽСԽ��׼, �����л�ʱ��Խ��
	* ������ת���ٵı���
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

	// �������ǰ�������ƶ�
	void MoveForward(float val);

	// ����������������ƶ�
	void MoveRight(float val);

	// ��� X �������� YAW ����ת
	void RotYaw(float val);

	// ��� Y �������� PITCH ����ת
	void RotPitch(float val);

	// ����۳�ʼ�ĳ���
	void InitArmLength();

	// ��ʼ����������ѡ����
	void InitControlRotation();

	// ʼ�������ڿ�������
	/************************************************************************/
	/* �����ڵĻ�������ɾ��, ��������򱳾���ĵ�һ�� actor.tag ��Ϊ "skysphere"  */
	/* �����ڵĵذ��һ�� actor.tag ��Ϊ "floor"							*/
	/* �����ڵ�ǽ���һ�� actor.tag ��Ϊ "wall"								*/
	/* �����ڵ�ȫ������� actor.tag ��Ϊ "pano"                             */
	/* �����ڵ�ȫ��ͼ��� actor.tag ��Ϊ "panopoint"						*/
	/************************************************************************/
	UFUNCTION(BlueprintCallable)
	void InitSceneActor();

	// --------------------------------------- UI ������� ------------------------------- //

	// �ı� Pawn ״̬ �ӽ��л�
	void ChangePawnState();

	// ����һ��ģ��
	// void SpawnNewMode(FString staticMeshPath);   // ���뵽 UI ��ʹ�������,�贫��һ��ģ���ļ���ַ
	UFUNCTION(BlueprintCallable)
	void SpawnNewMode(FString url, FString fileName, int32 gid);

	// ��ģ�͵�״̬��Ϊ���ƶ�״̬
	UFUNCTION(BlueprintCallable)
	void PawnBaseModeMove();

	// ɾ��ѡ�е�ģ��
	UFUNCTION(BlueprintCallable)
	void RemovePawnBaseMode();

	// ��������
	// void ChangeNewMateril(FString staticMeshPath);	// ���뵽 UI ��ʹ�������,�贫��һ�������ļ���ַ
	UFUNCTION(BlueprintCallable)
	void ChangeNewMateril(FString url, FString fileName, FName gid);

	// ��ȡ���е�ģ��
	// ���淽��ʱ��ʹ�� ����ģ��, �ذ�, ǰ��
	void LoadAllBaseActor();

	// ���㵥��ȫ��ͼ
	void PanoSimpleCap();

	// �������ȫ��ͼ
	void PanoAllCap();

	

	// ---------------------------------------------------------------------------------- //

	// ����ҽ����·���
	void MouseRightDown();

	// ����Ҽ�̧�𷽷�
	void MouseRightUp();

	// ���������·���
	UFUNCTION(BlueprintCallable)
	void MouseLeftDown();

	// ����Ϲ�����
	void MouseWheelUp();

	// ����¹�����
	void MouseWheelDown();

	// ��ģ�����ȡģ��
	void setBaseMode( class ABaseMode* mode);

	// �ذ屻����󴥷�����
	UFUNCTION()
		void FloorOnClick(AActor* TouchedActor, FKey ButtonPressed);

	// ǽ�汻����󴥷�����
	UFUNCTION()
		void WallOnClick(AActor* TouchedActor, FKey ButtonPressed);

	// ��ӫ���ϴ�ӡ�ַ���
	void OutPrint(FString myString);
	
	
	USoundWave* GetSoundWaveFromFile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundWave* sw;

	UFUNCTION(BlueprintImplementableEvent)
		void PlaySoundEvent();
};
