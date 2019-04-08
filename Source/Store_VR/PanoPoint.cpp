// Fill out your copyright notice in the Description page of Project Settings.

#include "PanoPoint.h"
#include "Components/ArrowComponent.h"
#include "Store_VRGameModeBase.h"

// Sets default values
APanoPoint::APanoPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneComp"));
	RootComponent = sceneComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(sceneComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowComp"));
	arrowComp->SetupAttachment(sceneComp);
}

// Called when the game starts or when spawned
void APanoPoint::BeginPlay()
{
	Super::BeginPlay();
	
	meshComp->SetHiddenInGame(true);
}

// Called every frame
void APanoPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

