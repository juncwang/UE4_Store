// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_modeSelect.h"
#include "CameraPlayerController.h"



void UCUI_modeSelect::initModeUI()
{
	ACameraPlayerController* controller = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
	float x, y;
	controller->GetMousePosition(x, y);
	SetPositionInViewport(FVector2D(x, y));
}


