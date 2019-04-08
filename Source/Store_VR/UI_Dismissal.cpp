// Fill out your copyright notice in the Description page of Project Settings.

#include "UI_Dismissal.h"
#include "rapidjson/document.h"
#include "CameraGameInstance.h"



void UUI_Dismissal::initUIDismissal()
{
	getDocDismissal();
}

void UUI_Dismissal::getDocDismissal()
{
	FString file = "RuntimeData/Dismissal.txt";
	FFileHelper::LoadFileToString(DocContent, *file);

	DismissalToBP();
}

void UUI_Dismissal::ChangeGameInstance()
{
	if (GetWorld())
	{
		cameraGameInstance = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		cameraGameInstance->emap = EMaps::Select;
	}
}
