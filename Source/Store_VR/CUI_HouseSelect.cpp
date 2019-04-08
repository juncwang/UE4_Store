// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_HouseSelect.h"
#include "CameraGameInstance.h"



void UCUI_HouseSelect::setHouseStyle(FString _houseStyle)
{
	HouseTypeData.HouseStyle = _houseStyle;
}

void UCUI_HouseSelect::initHouseSelect(FHouseType fHouseType, FHouseTypeData fHouseTypeData, FString houseGid)
{
	HouseType = fHouseType;
	HouseTypeData = fHouseTypeData;
	_houseGid = houseGid;

	EventUpdataUI();
}

void UCUI_HouseSelect::ChangeGameInstance()
{
	UCameraGameInstance* CGInstance;
	if (GetWorld())
	{
		CGInstance = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		if (CGInstance != nullptr)
		{
			CGInstance->emap = EMaps::Loading;
		}
	}
}

void UCUI_HouseSelect::log(FString myString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, myString);
	}
}
