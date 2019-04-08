// Fill out your copyright notice in the Description page of Project Settings.

#include "PanoActor.h"
#include "CameraPlayerController.h"
#include "CameraPawn.h"
#include "Store_VRGameModeBase.h"


// Sets default values
APanoActor::APanoActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APanoActor::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		CameraController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
		CameraPawn = Cast<ACameraPawn>(CameraController->GetPawn());
		gameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
	}
	
}

void APanoActor::panoPointArrIsNULL()
{
	panoPointArr.RemoveAt(0);

	if (panoPointArr.Num() > 0)
	{
		PanoAllStart();
	}
	else
	{
		ChangeControllerView();
	}
}

void APanoActor::ChangeControllerView()
{
	CameraPawn->bPanoCap = false;
	CameraPawn->SetActorLocation(FVector(CameraPawn->GetActorLocation().X + 0.01f, CameraPawn->GetActorLocation().Y, CameraPawn->GetActorLocation().Z));
	CameraController->SetViewTarget(CameraPawn);
	CameraController->FPointDataArray = pointDataArray;
	CameraController->UploadFileFromJPG();
}

