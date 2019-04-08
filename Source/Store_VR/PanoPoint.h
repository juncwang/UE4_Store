// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PanoPoint.generated.h"

UCLASS()
class STORE_VR_API APanoPoint : public AActor
{
	GENERATED_BODY()

public:

	/************************************************************************/
	/* ����ͼ�� actor.tag ����淶                                          */
	/* array[0] = panopoint ����Ϊ����ȫ��ͼ�����							*/
	/* array[1] = locationName λ������									*/
	/*          ��Ϊ�޷��� 3D ������ʹ������������Ҫʹ���ַ��淶����		*/
	/*			kt_1 ��ʾ����һ, �����Դ�����								*/
	/*			ct_1 ��ʾ����һ, �����Դ�����								*/
	/*			ws_1 ��ʾ����һ, �����Դ�����								*/
	/*			cf_1 ��ʾ����һ, �����Դ�����								*/
	/*			cs_1 ��ʾ����һ, �����Դ�����								*/
	/* array[2] = OtherlocationName_180 ��ָ��������������Ƽ���ת����		*/
	/*			ÿ��λ�ÿ��Թ������λ��, ���Կ��Լ����������������Ƽ�����*/
	/* array[3] = Other2locationName_90										*/
	/* array[n] = OtherNlocationName_45										*/
	/************************************************************************/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
		class USceneComponent* sceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
		class UStaticMeshComponent* meshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Comp")
		class UArrowComponent* arrowComp;
	
public:	
	// Sets default values for this actor's properties
	APanoPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
