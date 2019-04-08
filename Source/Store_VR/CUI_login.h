// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "CUI_login.generated.h"

/**
 * 
 */
UCLASS()
class STORE_VR_API UCUI_login : public UUserWidget
{
	GENERATED_BODY()
	
public:

	FString username;
	FString password;

	class AStore_VRGameModeBase* gameMode;
	class UCameraGameInstance* gameIns;

	UFUNCTION(BlueprintCallable)
		void LoginCallable(FString user, FString pass);

	// «Î«Û”√ªßµ«¬º
	void userLoginDetail();
	void LuserLoginDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	UFUNCTION(BlueprintImplementableEvent)
		void LoginSuccess();

	UFUNCTION(BlueprintImplementableEvent)
		void LoginFailure();

	void log(FString myString);
};
