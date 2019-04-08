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
	/* 在蓝图内 actor.tag 数组规范                                          */
	/* array[0] = panopoint 声明为序列全景图拍摄点							*/
	/* array[1] = locationName 位置名称									*/
	/*          因为无法在 3D 场景内使用中文所以需要使用字符规范代替		*/
	/*			kt_1 表示客厅一, 数字以此类推								*/
	/*			ct_1 表示餐厅一, 数字以此类推								*/
	/*			ws_1 表示卧室一, 数字以此类推								*/
	/*			cf_1 表示厨房一, 数字以此类推								*/
	/*			cs_1 表示厕所一, 数字以此类推								*/
	/* array[2] = OtherlocationName_180 所指向的其他区域名称及旋转度数		*/
	/*			每个位置可以关联多个位置, 所以可以加入多个其他区域名称及度数*/
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
