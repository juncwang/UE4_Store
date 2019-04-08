// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_mainPanoLoading.h"
#include "CameraPlayerController.h"



void UCUI_mainPanoLoading::initMainPanoLoading()
{
	if (GetWorld())
	{
		CameraPlayerController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
	}

	backgroundImage = CameraPlayerController->GetTexture("Image/bg_panoing.png");

	if (backgroundImage)
	{
		UpdataBGImage();
	}
}
