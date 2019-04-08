// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_main.h"
#include "CameraPawn.h"
#include "BaseMode.h"
#include "CameraGameInstance.h"
#include "Store_VRGameModeBase.h"
#include "CameraPlayerController.h"
#include "NetworkInterface.h"
#include "rapidjson/document.h"
using namespace rapidjson;

void UCUI_main::initMain()
{
	mainInitLoading();

	curPage = 1;
	pageSize = 14;
	BrandSelectGid = 0;
	StyleSelectGid = 0;
	TypeSelectGid = 0;
	if (GetWorld())
	{
		cameraPawn = Cast<ACameraPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		cameraInstance = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		gameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
		CameraPlayerController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());

		cameraPawn->ActorLocation = cameraPawn->GetActorLocation();
		cameraPawn->SetActorLocation(FVector::ZeroVector);
	}

	backgroundImage = CameraPlayerController->GetTexture("Image/logo.png");

	if (backgroundImage)
	{
		UpdataBGImage();
	}

	// 一个致命错误。等解决后开启这方法
	loadHOUSEINFOSchemeDetail();
	initMainBP();
}

void UCUI_main::ViewChange()
{
	cameraPawn->ChangePawnState();
}

void UCUI_main::ViewFirstChange()
{
	if (cameraPawn->PawnState == EPawnState::PawnSkyRotator)
	{
		cameraPawn->ChangePawnState();
	}
}

void UCUI_main::ChangeGameInstanceToLoading()
{
	if (cameraInstance != nullptr)
	{
		cameraInstance->emap = EMaps::Loading;
	}
}

void UCUI_main::PanoSimpleStart()
{
	gameMode->bAllPano = false;
}

void UCUI_main::PanoAllStart()
{
	gameMode->bAllPano = true;
}

void UCUI_main::AddMenuDown()
{
	loadBRANDSchemeDetail();
	loadSTYLESchemeDetail();
	loadTYPESchemeDetail();
	loadCommoditySchemeDetail();
}

void UCUI_main::log(FString myString)
{
	if (GEngine)
	{
		myString = "UCUI_main::" + myString;
		GEngine->AddOnScreenDebugMessage(-1, 9.0f, FColor::Green, myString);
	}
}

void UCUI_main::SetBrandType(int32 num)
{
	if (BRANDType != num)
	{
		BRANDType = num;
		curPage = 1;
	}
}

void UCUI_main::loadBRANDSchemeDetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += BRAND_LIST_INTERFACE + FString::FromInt(BRANDType);
	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_main::LoadBRANDSchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_main::LoadBRANDSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		//成功获取网络返回的数据
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);
		BRANDName.Empty();
		BRANDGid.Empty();

		if (jsonDoc["code"].GetInt() == 1000)
		{
			int32 length = jsonDoc["total"].GetInt();
			for (int32 i=0; i<length; i++)
			{
				BRANDName.Push(jsonDoc["data"][i]["brandName"].GetFString());
				BRANDGid.Push(jsonDoc["data"][i]["gid"].GetInt());
			}
			UpdataPinPai();
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
	
}

void UCUI_main::loadSTYLESchemeDetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += STYLE_LIST_INTERFACE;
	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_main::LoadSTYLESchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_main::LoadSTYLESchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		//成功获取网络返回的数据
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);
		STYLEName.Empty();
		STYLEGid.Empty();

		if (jsonDoc["code"].GetInt() == 1000)
		{
			int32 length = jsonDoc["total"].GetInt();
			for (int32 i = 0; i < length; i++)
			{
				STYLEName.Push(jsonDoc["data"][i]["styleName"].GetFString());
				STYLEGid.Push(jsonDoc["data"][i]["gid"].GetInt());
			}
			UpdataStyle();
		}
		
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
}

void UCUI_main::loadTYPESchemeDetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += TYPE_LIST_INTERFACE + FString::FromInt(BRANDType);
	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_main::LoadTYPESchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_main::LoadTYPESchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		//成功获取网络返回的数据
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);
		TYPEName.Empty();
		TYPEGid.Empty();

		if (jsonDoc["code"].GetInt() == 1000)
		{
			int32 length = jsonDoc["total"].GetInt();
			for (int32 i = 0; i < length; i++)
			{
				TYPEName.Push(jsonDoc["data"][i]["typeName"].GetFString());
				TYPEGid.Push(jsonDoc["data"][i]["gid"].GetInt());
			}
			UpdataType();
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
}

void UCUI_main::UpdataCommodity()
{
	
	loadCommoditySchemeDetail();
}

void UCUI_main::loadCommoditySchemeDetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += COMMODITY_LIST_INTERFACE;
	url += "?curPage=" + FString::FromInt(curPage) + "&pageSize=" + FString::FromInt(pageSize) + "&typeParentGid=" + FString::FromInt(BRANDType);
	if (!TypeSelectGid == 0)
	{
		url += "&typeGid=" + FString::FromInt(TypeSelectGid);
	}
	if (!BrandSelectGid==0)
	{
		url += "&brandGid=" + FString::FromInt(BrandSelectGid);
	}
	if (!StyleSelectGid == 0)
	{
		url += "&styleGid=" + FString::FromInt(StyleSelectGid);
	}
	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_main::LoadCommoditySchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

#define print(Format, ...) { UE_LOG(LogClass, Warning, Format, ##__VA_ARGS__); }
void UCUI_main::LoadCommoditySchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		//成功获取网络返回的数据
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);
		imgURLArray.Empty();
		_fileIdArray.Empty();
		FilePathArray.Empty();
		gidArray.Empty();
		DownPathArray.Empty();
		FileNameArray.Empty();

		print(TEXT("%s"), *MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			int32 length = int32(jsonDoc["data"].GetArray().Size());

			for (int32 i = 0; i < length; i++)
			{
				imgURLArray.Push(jsonDoc["data"][i]["productImageUrl"].GetFString());
				gidArray.Push(jsonDoc["data"][i]["gid"].GetInt());
				_fileIdArray.Push(FString::FromInt(jsonDoc["data"][i]["productVersion"].GetInt()));

				if (BRANDType == 1)
				{
					FilePathArray.Push(jsonDoc["data"][i]["modelFolderUrl"].GetFString());
					FileNameArray.Push(jsonDoc["data"][i]["modelFileName"].GetFString());
					DownPathArray.Push(jsonDoc["data"][i]["modelInfoFileUrl"].GetFString());
				}
				else {
					FilePathArray.Push(jsonDoc["data"][i]["materialFolderUrl"].GetFString());
					FileNameArray.Push(jsonDoc["data"][i]["materialFileName"].GetFString());
					DownPathArray.Push(jsonDoc["data"][i]["materialInfoFileUrl"].GetFString());
				}
			}

			if (curPage <= 1)
			{
				isPrevPage = false;
			}
			else {
				isPrevPage = true;
			}

			if (curPage * pageSize >= jsonDoc["total"].GetInt())
			{
				isNextPage = false;
			}
			else
			{
				isNextPage = true;
			}


			UpadataComList();
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
	}
}

void UCUI_main::initMainData()
{
	log("void UCUI_main::initMainData()");
	// 装载 json 内模型材质数据-----------------------------------------------------------
	if (FCString::Atoi(*cameraInstance->gidScheme) > 1)
	{
		FString result;
		FString file = FPaths::ProjectContentDir() + "/ScenarioConfigurationFile.json";
		FFileHelper::LoadFileToString(result, *file);
		Document jsonDoc;
		jsonDoc.Parse(result);

		// 装载 json 内的所有模型文件

		int32 length = int32(jsonDoc["model"].GetArray().Size());

		for (int32 i = 0; i < length; i++) {
			FString gid = jsonDoc["model"][i]["gid"].GetFString();
			FVector location = FVector::ZeroVector;
			location.X = jsonDoc["model"][i]["transform"]["location"]["x"].GetFloat();
			location.Y = jsonDoc["model"][i]["transform"]["location"]["y"].GetFloat();
			location.Z = jsonDoc["model"][i]["transform"]["location"]["z"].GetFloat();
			FRotator rotation = FRotator::ZeroRotator;
			rotation.Roll = jsonDoc["model"][i]["transform"]["rotation"]["roll"].GetFloat();
			rotation.Pitch = jsonDoc["model"][i]["transform"]["rotation"]["pitch"].GetFloat();
			rotation.Yaw = jsonDoc["model"][i]["transform"]["rotation"]["yaw"].GetFloat();
			FVector scale = FVector::ZeroVector;
			scale.X = jsonDoc["model"][i]["transform"]["scale"]["x"].GetFloat();
			scale.Y = jsonDoc["model"][i]["transform"]["scale"]["y"].GetFloat();
			scale.Z = jsonDoc["model"][i]["transform"]["scale"]["z"].GetFloat();

			for (int32 j = 0; j < cameraInstance->_modelGidArray.Num(); j++)
			{
				if (gid.Equals(cameraInstance->_modelGidArray[j]))
				{
					FString path = cameraInstance->_modelFilePathArray[j];
					FString filename = cameraInstance->_modelFileNameArray[j];
					FString staticMeshPath = "StaticMesh'/Game/" + path + "/" + filename + "." + filename + "'";

					ABaseMode* baseMode = GetWorld()->SpawnActor<ABaseMode>(ABaseMode::StaticClass(), location, rotation);
					UStaticMesh* mesh = LoadObject<UStaticMesh>(NULL, *staticMeshPath);
					if (baseMode && mesh)
					{
						baseMode->initStaticMesh(mesh, FCString::Atoi(*gid));
						baseMode->BaseModeState = EBaseModeState::BaseModeStatic;
					}
					break;
				}
			}
		}

		// 装载 json 内的所有材质文件
		length = int32(jsonDoc["material"].GetArray().Size());

		cameraPawn->InitSceneActor();

		for (int32 i = 0; i < length; i++)
		{
			FString gid = jsonDoc["material"][i]["gid"].GetFString();
			FString modelName = jsonDoc["material"][i]["modelName"].GetFString();
			log("in json material : " + modelName);
			for (int32 j = 0; j < cameraInstance->_materialGidArray.Num(); j++)
			{
				bool isRun = true;
				if (gid.Equals(cameraInstance->_materialGidArray[j]))
				{
					FString path = cameraInstance->_materialFilePathArray[j];
					FString filename = cameraInstance->_materialFileNameArray[j];
					log("in for Camera if true : " + cameraInstance->_materialFileNameArray[j]);
					log("floor Actor is num : " + cameraPawn->FloorActorArr.Num());

					for (AActor* actor : cameraPawn->FloorActorArr)
					{
						log("model name is " + actor->GetName());

						if (modelName.Equals(actor->GetName()))
						{
							log("model name is true =====================================================");
							UStaticMeshComponent* actorComp;
							FString materialPath = "Material'/Game/" + path + "/" + filename + "." + filename + "'";
							UMaterial* material = LoadObject<UMaterial>(NULL, *materialPath);
							actor->Tags.Push(FName(*gid));

							TArray<UStaticMeshComponent*> StaticMeshComArr;
							actor->GetComponents(StaticMeshComArr, false);

							if (StaticMeshComArr.Num() > 0)
							{
								actorComp = StaticMeshComArr[0];
								actorComp->SetMaterial(0, material);
								isRun = false;
								break;
							}
						}
					}

					for (AActor* actor : cameraPawn->WallActorArr)
					{
						if (!isRun)
						{
							break;
						}

						if (modelName.Equals(actor->GetName()))
						{
							UStaticMeshComponent* actorComp;
							FString materialPath = "Material'/Game/" + path + "/" + filename + "." + filename + "'";
							UMaterial* material = LoadObject<UMaterial>(NULL, *materialPath);
							actor->Tags.Push(FName(*gid));

							TArray<UStaticMeshComponent*> StaticMeshComArr;
							actor->GetComponents(StaticMeshComArr, false);

							if (StaticMeshComArr.Num() > 0)
							{
								actorComp = StaticMeshComArr[0];
								actorComp->SetMaterial(0, material);
								isRun = false;
								break;
							}
						}

					}
				}
				if (!isRun)
				{
					break;
				}
			}
		}
	}
	log("void UCUI_main::initMainData()");
	mainCloseLoading();
	cameraPawn->InitSceneActor();
}

void UCUI_main::loadHOUSEINFOSchemeDetail()
{
	log("void UCUI_main::loadHOUSEINFOSchemeDetail()");
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += HOUSEINFO_INTERFACE;
	url += cameraInstance->houseGid;
	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_main::LoadHOUSEINFOSchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_main::LoadHOUSEINFOSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	log("void UCUI_main::loadHOUSEINFOSchemeDetail()");
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		//成功获取网络返回的数据
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);
		areaNameArray.Empty();
		areaSizeArray.Empty();

		if (jsonDoc["code"].GetInt() == 1000)
		{
			deliveryArea = jsonDoc["data"]["deliveryArea"].GetFString();

			int32 length = int32(jsonDoc["data"]["areaInfoList"].GetArray().Size());

			for (int32 i = 0; i < length; i++)
			{
				areaNameArray.Push(jsonDoc["data"]["areaInfoList"][i]["areaName"].GetFString());
				areaSizeArray.Push(jsonDoc["data"]["areaInfoList"][i]["areaSize"].GetFString());
			}

			UpdataHouseInfo();
			loadROOMLISTSchemeDetail();
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
	}
}

void UCUI_main::loadROOMLISTSchemeDetail()
{
	log("void UCUI_main::loadROOMLISTSchemeDetail()");
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += ROOMLIST_INFO_INTERFACE;
	url += cameraInstance->houseGid;
	HttpRequest->SetURL(url);

	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_main::LoadROOMLISTSchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_main::LoadROOMLISTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	log("void UCUI_main::loadROOMLISTSchemeDetail()");
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		initMainData();
		//成功获取网络返回的数据
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			FHouseType houseType;
			houseType.BOConfigStyleGid = 0;
			houseType.BOSchemeGid = 0;
			houseType.BOGid = 0;
			houseType.DNYConfigStyleGid = 0;
			houseType.DNYSchemeGid = 0;
			houseType.DNYGid = 0;
			houseType.XDConfigStyleGid = 0;
			houseType.XDSchemeGid = 0;
			houseType.XDGid = 0;
			houseType.XMConfigStyleGid = 0;
			houseType.XMSchemeGid = 0;
			houseType.XMGid = 0;
			houseType.XZSConfigStyleGid = 0;
			houseType.XZSSchemeGid = 0;
			houseType.XZSGid = 0;

			for (int32 j = 0; j < int32(jsonDoc["data"].GetArray().Size()); j++)
			{
				int32 num = jsonDoc["data"][j]["configStyleGid"].GetInt();
				int32 gid = jsonDoc["data"][j]["gid"].GetInt();
				int32 schemeGid = jsonDoc["data"][j]["defaultSchemeGid"].GetInt();
				switch (num)
				{
				case 17:
					houseType.XDConfigStyleGid = num;
					houseType.XDGid = gid;
					houseType.XDSchemeGid = schemeGid;
					break;
				case 18:
					houseType.XZSConfigStyleGid = num;
					houseType.XZSGid = gid;
					houseType.XZSSchemeGid = schemeGid;
					break;
				case 19:
					houseType.BOConfigStyleGid = num;
					houseType.BOGid = gid;
					houseType.BOSchemeGid = schemeGid;
					break;
				case 20:
					houseType.XMConfigStyleGid = num;
					houseType.XMGid = gid;
					houseType.XMSchemeGid = schemeGid;
					break;
				case 21:
					houseType.DNYConfigStyleGid = num;
					houseType.DNYGid = gid;
					houseType.DNYSchemeGid = schemeGid;
					break;
				default:
					break;
				}
				HouseType = houseType;
				
			}
		}
	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
	}
}

void UCUI_main::updataPrice(float _price)
{
	price = _price;
	EventPrice();
}
