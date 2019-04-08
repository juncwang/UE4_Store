// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <fstream>
#include <iomanip>

/**
 * 
 */
class STORE_VR_API Tools
{
public:
	static void saveDataToFile(FString filePath, TArray<uint8> tdata);
	static bool system_hide(TCHAR* CommandLine);
	static bool system_hide_nowait(TCHAR* CommandLine);
};
