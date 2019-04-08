// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_materialSelect.h"
#include "CameraPlayerController.h"



void UCUI_materialSelect::initMaterialUI()
{
	ACameraPlayerController* controller = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
	float x, y;
	controller->GetMousePosition(x, y);
	SetPositionInViewport(FVector2D(x, y));
}
