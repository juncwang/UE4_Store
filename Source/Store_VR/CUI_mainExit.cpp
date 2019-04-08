// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_mainExit.h"
#include "CameraGameInstance.h"



void UCUI_mainExit::initExitUI()
{

}

void UCUI_mainExit::ChangeGameInstance()
{
	if (GetWorld())
	{
		cameraGameInstance = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		cameraGameInstance->emap = EMaps::Select;
	}
}
