// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_mainCore.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_mainCore : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// ≥ı ºªØ UI
	void initMainCore();
	
	class ACameraPlayerController* CameraPC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* textureBG;

	UFUNCTION(BlueprintImplementableEvent)
		void ChangeCodeImage();

	UTexture2D* GetTexture(FString url);

	void log(FString myString);
};
