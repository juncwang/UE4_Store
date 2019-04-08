// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUI_select.h"
#include "CUI_search.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_search : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString searchBtn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString url;

	UFUNCTION(BlueprintCallable)
		void SetFSearch(FSearchData searchData);
	
	UFUNCTION(BlueprintImplementableEvent)
		void EventSetText();
};
