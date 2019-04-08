// Fill out your copyright notice in the Description page of Project Settings.

#include "Store_VRGameModeBase.h"
#include "CameraPawn.h"
#include "CameraPlayerController.h"
#include "CameraGameInstance.h"
#include "windows.h"
#include "ThirdParty/CryptoPP/5.6.5/include/cryptlib.h"



void AStore_VRGameModeBase::SaveProgram()
{
	// 需把本地的方案保存到数据库内, 方便以后调用
}

void AStore_VRGameModeBase::CreateNetErrorUI()
{

		if (netErrorUI)
		{
			netErrorUI->SetVisibility(ESlateVisibility::Visible);
			netErrorUI->initNetErrorBP();

// 			if (T_netErrorUI)
// 			{
// 				netErrorUI = CreateWidget<UCUI_netError>(GetWorld(), T_netErrorUI);
// 				if (netErrorUI)
// 				{
// 					netErrorUI->AddToViewport();
// 					netErrorUI->initNetErrorBP();
// 				}
// 			}
		}
}

void AStore_VRGameModeBase::RemoveNetErrorUI()
{
	if (netErrorUI)
	{
		netErrorUI->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AStore_VRGameModeBase::mainCoreUI()
{
	if (T_mainCore != nullptr && GetWorld())
	{
		mainCore = CreateWidget<UCUI_mainCore>(GetWorld(), T_mainCore);
		mainCore->AddToViewport();
		mainCore->initMainCore();
	}
}

void AStore_VRGameModeBase::MaterialMenu()
{
	if (ui_main)
	{
		ui_main->MaterialMenu();
	}
}

void AStore_VRGameModeBase::CloseAllMenu()
{
	ui_main->CloseAllMenu();
}

void AStore_VRGameModeBase::openSaveTitle()
{
	if (ui_main)
	{
		ui_main->openSaveTitle();
	}
	
}

void AStore_VRGameModeBase::removeMainPanoLoading()
{
	if (mainPanoLoading != nullptr)
	{
		mainPanoLoading->RemoveFromParent();
	}
}

void AStore_VRGameModeBase::LoadPanoUI()
{
	if (T_mainPanoLoading && GetWorld())
	{
		mainPanoLoading = CreateWidget<UCUI_mainPanoLoading>(GetWorld(), T_mainPanoLoading);
		mainPanoLoading->AddToViewport();
		mainPanoLoading->initMainPanoLoading();

		if (bAllPano)
		{
			cameraPawn->PanoAllCap();
		}
		else {
			cameraPawn->PanoSimpleCap();
		}


	}
}

AStore_VRGameModeBase::AStore_VRGameModeBase()
{
	DefaultPawnClass = ACameraPawn::StaticClass();
	PlayerControllerClass = ACameraPlayerController::StaticClass();

	netErrorUI = nullptr;
}

void AStore_VRGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	bErrorQuit = false;
	if (GetWorld())
	{
		CGInstance = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		switch (CGInstance->emap)
		{
		case EMaps::Index:
			LoadIndexUI();
			break;
		case EMaps::Select:
			LoadSelectUI();
			break;
		case EMaps::Loading:
			LoadLoadingUI();
			break;
		case EMaps::MainPage:
			LoadMainPageUI();
			break;
		default:
			break;
		}

		if (T_netErrorUI)
		{
			netErrorUI = CreateWidget<UCUI_netError>(GetWorld(), T_netErrorUI);
			if (netErrorUI)
			{
				netErrorUI->AddToViewport();
				netErrorUI->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		cameraPawn = Cast<ACameraPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
}

FString AStore_VRGameModeBase::GetFileDir(bool isShipping)
{
	FString dir = "";
	
	if (UE_BUILD_SHIPPING)
	{
		const int MAX_BUFFER_LEN = 500;
		DWORD dwNameLen;
		dwNameLen = 500;

		TCHAR name[MAX_BUFFER_LEN];

		GetUserName(name, &dwNameLen);

		dir = "C:/Users/";
		dir += name;
		dir += "/AppData/Local/Store_VR/Saved/Screenshots/WindowsNoEditor/";
	}
	else if (UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT)
	{
		dir = "../../Saved/Screenshots/WindowsNoEditor/";
	}
	else
	{
		// 非生产版本文件路径跟随项目路径 ?:/.../Store_VR/Saved/Screenshots/Windows/
		dir = "D:/Project/Store_VR/Saved/Screenshots/Windows/";
	}
	return dir;
}

void AStore_VRGameModeBase::LoadIndexUI()
{
	if (Tui_index != nullptr)
	{
		ui_index = CreateWidget<UUI_index>(GetWorld(), Tui_index);
		if (ui_index != nullptr)
		{
			ui_index->AddToViewport();
			ui_index->initIndex();
		}
	}
}

void AStore_VRGameModeBase::LoadSelectUI()
{
	if (Tui_select != nullptr)
	{
		ui_select = CreateWidget<UCUI_select>(GetWorld(), Tui_select);
		if (ui_select != nullptr)
		{
			ui_select->AddToViewport();
			ui_select->initSelect();
		}
	}
}

void AStore_VRGameModeBase::LoadLoadingUI()
{
	if (Tui_loading != nullptr)
	{
		ui_loading = CreateWidget<UCUI_loading>(GetWorld(), Tui_loading);
		if (ui_loading != nullptr)
		{
			ui_loading->AddToViewport();
			ui_loading->initLoading();
		}
	}
}

void AStore_VRGameModeBase::LoadMainPageUI()
{
	if (Tui_main != nullptr)
	{
		ui_main = CreateWidget<UCUI_main>(GetWorld(), Tui_main);
		if (ui_main != nullptr)
		{
			ui_main->AddToViewport();
 			ui_main->initMain();
			addPrice = 0.0f;
		}
	}
}

void AStore_VRGameModeBase::UpdataMatinPage(float price)
{
	if (ui_main != nullptr)
	{
		addPrice += price;
		ui_main->updataPrice(addPrice);
	}
}

void AStore_VRGameModeBase::runSelectUISearch(FText submitText, FText btnName)
{
	ui_select->changeTextBox(btnName);
	ui_select->submitSearch(submitText);
}

void AStore_VRGameModeBase::isShowHouseData()
{
	ui_main->isShowHouseData();
}

void AStore_VRGameModeBase::SetBrandType(int32 num)
{
	if (ui_main)
	{
		ui_main->SetBrandType(num);
	}
}

void AStore_VRGameModeBase::log(FString mystring)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Orange, mystring);
	}
}
