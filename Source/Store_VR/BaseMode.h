// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMode.generated.h"

UENUM()
enum class EBaseModeState: uint8
{
	BaseModeMove,	// �ƶ�״̬
	BaseModeSelect,	// ѡ��״̬
	BaseModeStatic,	// ��ֹ״̬
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

	// �������, ��������ڳ����е�λ�ù�ϵ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseActor")
		USceneComponent* SceneComp;

	// ģ�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseActor")
		UStaticMeshComponent* StaticMeshComp;

	// ģ��״̬
	EBaseModeState BaseModeState;

	// �Ƿ���������ѡ��ģ��ʱ
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

	// �����������¼�
	UFUNCTION()
	void actorOnClicked(AActor* TouchedActor, FKey ButtonPressed);

	// ��ʼ��ģ������
	void initStaticMesh(UStaticMesh *mesh, int32 id);

	// ����ѡ��״̬����ʾ
	void setRender(bool isShow);

	// �ڳ�����ɾ���Լ�
	void destroySelf();

	// ӫ����ӡ
	void OutPrint(FString myString);
};
