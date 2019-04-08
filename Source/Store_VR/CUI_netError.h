// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_netError.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_netError : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// ¿∂Õº≥ı ºªØ
	UFUNCTION(BlueprintImplementableEvent)
	void initNetErrorBP();
	
	
};
