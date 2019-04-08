// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_loading.h"
#include "Kismet/GameplayStatics.h"
#include "CameraPlayerController.h"
#include "CameraGameInstance.h"
#include "Store_VRGameModeBase.h"
#include "NetworkInterface.h"
#include "rapidjson/document.h"
#include "HAL/PlatformFilemanager.h"
#include "Tools.h"

using namespace rapidjson;

#define print(Format, ...) { UE_LOG(LogClass, Warning, Format, ##__VA_ARGS__); }

void UCUI_loading::mainInitLoading()
{
	if (GetWorld())
	{
		CameraPlayerController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
	}

	backgroundImage = CameraPlayerController->GetTexture("Image/Loading_Background.png");

	if (backgroundImage)
	{
		UpdataBG();
	}
	numLoading = 100.0f;
	textLoading = "100 %";
	EventChange();
}

void UCUI_loading::mainCloseLoading()
{
	this->RemoveFromParent();
}

void UCUI_loading::initLoading()
{
	
	if (GetWorld())
	{
		CameraPlayerController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
		cameraGameInstance = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		gameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	backgroundImage = CameraPlayerController->GetTexture("Image/Loading_Background.png");

	if (backgroundImage)
	{
		UpdataBG();
	}

	gid = cameraGameInstance->gid;
	gidScheme = cameraGameInstance->gidScheme;
	numLoading = 0;
	isJsonFile = true;
	isMapFile = true;
	prevNum = 0;

	textLoading = FString::FromInt(int32(numLoading)) + " %";
	GetSchemeJson();
}

bool UCUI_loading::isVerifyFile(const FString& Path)
{
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*Path))
	{
		return true;
	}
	return false;
}

void UCUI_loading::ChangeGameInstance()
{
 	cameraGameInstance->_modelGidArray = _modelGidArray;
	cameraGameInstance->_materialGidArray = _materialGidArray;

	log("==================================cameraGameInstance->_modelGidArray " + FString::FromInt(cameraGameInstance->_modelGidArray.Num()));
	log("==================================cameraGameInstance->_modelFileNameArray " + FString::FromInt(cameraGameInstance->_modelFileNameArray.Num()));
	log("==================================cameraGameInstance->_materialGidArray " + FString::FromInt(cameraGameInstance->_materialGidArray.Num()));
	log("==================================cameraGameInstance->_materialFileNameArray " + FString::FromInt(cameraGameInstance->_materialFileNameArray.Num()));


	cameraGameInstance->emap = EMaps::MainPage;
}


void UCUI_loading::log(FString myString)
{
	if (GEngine)
	{
		myString = "UI_loading::" + myString;
		GEngine->AddOnScreenDebugMessage(-1, 90, FColor::Orange, myString);
	}
}

/************************************************************************/
/*                                                                      */
/************************************************************************/


void UCUI_loading::GetSchemeJson()
{
	_modelGidArray.Empty();
	_modelFileNameArray.Empty();
	_modelFilePathArray.Empty();
	_modelFileDownUrlArray.Empty();
	_modelFileIDArray.Empty();

	_materialGidArray.Empty();
	_materialFileNameArray.Empty();
	_materialFilePathArray.Empty();
	_materialFileDownUrlArray.Empty();
	_materialFileIDArray.Empty();



	/************************************************************************/
	/* 应该是大于 1 后台给了个 2 又没有 json 文件                                                                     */
	/************************************************************************/


	

	if (FCString::Atoi(*gidScheme) > 1)
	{
		loadSCHEMEINFODetail();
	}
	else
	{
		CalculatedRatio();
	}
}

void UCUI_loading::loadSCHEMEINFODetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += SCHEME_INFO_INTERFACE;
	url += gidScheme;

	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_loading::LoadSCHEMEINFODetailCompleteCallback);
	HttpRequest->ProcessRequest();
}

void UCUI_loading::LoadSCHEMEINFODetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);
		if (jsonDoc["code"].GetInt() == 1000)
		{
			_FileDownUrl = jsonDoc["data"]["textFileUrl"].GetFString();
			print(TEXT("_FileDownUrl is : "), *_FileDownUrl);
			downloadMesh(_FileDownUrl);
		}

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
}

void UCUI_loading::CalculatedRatio()
{

	// 数组去重
	for (int32 i = 0; i < _modelGidArray.Num(); i++) {
		for (int32 j = i + 1; j < _modelGidArray.Num(); j++) {
			if (_modelGidArray[i].Equals(_modelGidArray[j])) {
				_modelGidArray.RemoveAt(j);
				j--;
			}
		}
	}

	// 数组去重
	for (int32 i = 0; i < _materialGidArray.Num(); i++) {
		for (int32 j = i + 1; j < _materialGidArray.Num(); j++) {
			if (_materialGidArray[i].Equals(_materialGidArray[j])) {
				_materialGidArray.RemoveAt(j);
				j--;
			}
		}
	}

// 	for (int32 i = 0; i < cameraGameInstance->_modelGidArray.Num(); i++) {
// 		for (int32 j = i + 1; j < cameraGameInstance->_modelGidArray.Num(); j++) {
// 			if (i != j)
// 			{
// 				if (cameraGameInstance->_modelGidArray[i].Equals(cameraGameInstance->_modelGidArray[j])) {
// 					cameraGameInstance->_modelGidArray.RemoveAt(j);
// 				}
// 			}
// 		}
// 	}
// 
// 	for (int32 i = 0; i < cameraGameInstance->_materialGidArray.Num(); i++) {
// 		for (int32 j = i + 1; j < cameraGameInstance->_materialGidArray.Num(); j++) {
// 			if (i != j)
// 			{
// 				if (cameraGameInstance->_materialGidArray[i].Equals(cameraGameInstance->_materialGidArray[j])) {
// 					cameraGameInstance->_materialGidArray.RemoveAt(j);
// 				}
// 			}
// 		}
// 	}


	isJsonFile = false;
	// 计算总大小, 模型数量 + 材质数量 + 地图(假设一个地图的大小为20个模型的大小或材质大小)
	numSize = _modelGidArray.Num() + _materialGidArray.Num() + 20;
	simModelNum = 100.0f / numSize;
	loadROOMTYPEDetail();
}

void UCUI_loading::StartModeData()
{
	log("void UCUI_loading::StartModeData()");

	if (numSize > 0)
	{
		FString gidArray1 = "";
		for (int32 i = 0; i < _modelGidArray.Num(); i++)
		{
			if (i == 0)
			{
				gidArray1 += _modelGidArray[i];
			}
			else
			{
				gidArray1 = gidArray1 + "," + _modelGidArray[i];
			}
		}
		FString gidArray2 = "";
		for (int32 i = 0; i < _materialGidArray.Num(); i++)
		{
			if (i == 0)
			{
				gidArray2 += _materialGidArray[i];
			}
			else
			{
				gidArray2 = gidArray2 + "," + _materialGidArray[i];
			}
		}
		if (!gidArray1.Equals("") || !gidArray2.Equals(""))
		{
			loadMODELINFDetail(gidArray1,gidArray2);
		}
		else {
			CleckFile();
		}
		
	}
	else {

		numLoading = 100.0f;
		textLoading = FString::FromInt(int32(numLoading)) + " %";
		EventChange();
		openMap();
	}
}

void UCUI_loading::loadMODELINFDetail(FString gidArray1,FString gidArray2)
{
	log("void UCUI_loading::loadMODELINFDetail(FString gidArray1,FString gidArray2)");
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += MODELINFOBYGID_INTERFACE;
	url += gidArray1;
	url += "&productMaterialGids=";
	url += gidArray2;

	print(TEXT("loadMODELINFDetail is : %s"), *url);

	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_loading::LoadMODELINFDetailCompleteCallback);
	HttpRequest->ProcessRequest();
}

void UCUI_loading::LoadMODELINFDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	log("void UCUI_loading::LoadMODELINFDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)");
	FString MessageBody = "";
	
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);
		cameraGameInstance->_modelFileNameArray.Empty();
		cameraGameInstance->_modelFilePathArray.Empty();
		cameraGameInstance->_materialFileNameArray.Empty();
		cameraGameInstance->_materialFilePathArray.Empty();
		if (jsonDoc["code"].GetInt() == 1000)
		{
			int32 num = int32(jsonDoc["model"].GetArray().Size());
			for (int32 i = 0; i<num ; i++)
			{
				_modelFileNameArray.Push(jsonDoc["model"][i]["fileName"].GetFString());
				cameraGameInstance->_modelFileNameArray.Push(jsonDoc["model"][i]["fileName"].GetFString());
				_modelFilePathArray.Push(jsonDoc["model"][i]["folderPath"].GetFString());
				cameraGameInstance->_modelFilePathArray.Push(jsonDoc["model"][i]["folderPath"].GetFString());
				_modelFileDownUrlArray.Push(jsonDoc["model"][i]["fileUrl"].GetFString());
				_modelFileIDArray.Push(FString::FromInt(jsonDoc["model"][i]["version"].GetInt()));
			}
			num = int32(jsonDoc["material"].GetArray().Size());
			for (int32 i = 0; i < num; i++)
			{
				_materialFileNameArray.Push(jsonDoc["material"][i]["fileName"].GetFString());
				cameraGameInstance->_materialFileNameArray.Push(jsonDoc["material"][i]["fileName"].GetFString());
				_materialFilePathArray.Push(jsonDoc["material"][i]["folderPath"].GetFString());
				cameraGameInstance->_materialFilePathArray.Push(jsonDoc["material"][i]["folderPath"].GetFString());
				_materialFileDownUrlArray.Push(jsonDoc["material"][i]["fileUrl"].GetFString());
				_materialFileIDArray.Push(FString::FromInt(jsonDoc["material"][i]["version"].GetInt()));
				
				

			}

			CleckFile();
		}

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
}

void UCUI_loading::StartMaterialData()
{
	FString gidArray = "";
	for (int32 i = 0; i < _materialGidArray.Num(); i++)
	{
		if (i == 0)
		{
			gidArray += _materialGidArray[i];
		}
		else
		{
			gidArray = gidArray + "," + _materialGidArray[i];
		}
	}
	if (!gidArray.Equals(""))
	{
		loadMATERIALINFODetail(gidArray);
	}
	else {
		CleckFile();
	}

// 	if (numSize > 0)
// 	{
// 		FString gidArray = "";
// 		for (int32 i = 0; i < _modelGidArray.Num(); i++)
// 		{
// 			if (i == 0)
// 			{
// 				gidArray += _modelGidArray[i];
// 			}
// 			else
// 			{
// 				gidArray = gidArray + "," + _modelGidArray[i];
// 			}
// 		}
// 		if (!gidArray.Equals(""))
// 		{
// 			loadMODELINFDetail(gidArray);
// 		}
// 		else {
// 			StartMaterialData();
// 		}
// 
// 	}
// 	else {
// 		numLoading = 100.0f;
// 		textLoading = FString::FromInt(int32(numLoading)) + " %";
// 		EventChange();
// 		openMap();
// 	}
}

void UCUI_loading::loadMATERIALINFODetail(FString gidArray)
{
// 	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
// 	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
// 	FString url = INTERFACE_ADDRESS;
// 	url += MATERIALINFOBYGID_INTERFACE;
// 	url += gidArray;
// 
// 	HttpRequest->SetURL(url);
// 	HttpRequest->SetVerb(TEXT("GET"));
// 	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_loading::LoadMATERIALINFODetailCompleteCallback);
// 	HttpRequest->ProcessRequest();

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += MATERIALINFOBYGID_INTERFACE;
	url += gidArray;

	print(TEXT("url : %s"), *url);

	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_loading::LoadMATERIALINFODetailCompleteCallback);
	HttpRequest->ProcessRequest();
}

void UCUI_loading::LoadMATERIALINFODetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);
		if (jsonDoc["code"].GetInt() == 1000)
		{
			int32 num = int32(jsonDoc["data"].GetArray().Size());
			for (int32 i = 0; i < num; i++)
			{
				_materialFileNameArray.Push(jsonDoc["data"][i]["fileName"].GetFString());
				_materialFilePathArray.Push(jsonDoc["data"][i]["folderPath"].GetFString());
				_materialFileDownUrlArray.Push(jsonDoc["data"][i]["fileUrl"].GetFString());
			}
			CleckFile();
		}

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
}

void UCUI_loading::CleckFile()
{
// 	_modelFileNameArray;
// 	_modelFilePathArray;
// 	_modelFileDownUrlArray;
// 
// 	_materialFileNameArray;
// 	_materialFilePathArray;
// 	_materialFileDownUrlArray;

	
	isMapFile = false;
	if (numSize > 0)
	{
		if (_modelFileNameArray.Num() > 0)
		{
			_FileName = _modelFileNameArray[0];
			_FilePath = _modelFilePathArray[0];
			_FileDownUrl = _modelFileDownUrlArray[0];
			_FileId = _modelFileIDArray[0];

			_modelFileNameArray.RemoveAt(0);
			_modelFilePathArray.RemoveAt(0);
			_modelFileDownUrlArray.RemoveAt(0);
			_modelFileIDArray.RemoveAt(0);

			FString filePathId = FPaths::ProjectContentDir() + _FilePath + "/" + _FileId;
			FString filePathName = FPaths::ProjectContentDir() + _FilePath + "/" + _FileName + ".uasset";

			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			if (PlatformFile.FileExists(*filePathName) && PlatformFile.FileExists(*filePathId))
			{
				numSize--;
				numLoading += simModelNum;
				textLoading = FString::FromInt(int32(numLoading)) + " %";
				EventChange();
				CleckFile();
			}
			else
			{
				print(TEXT("file down url : %s"), *_FileDownUrl);
				downloadMesh(_FileDownUrl);
				
			}
		}
		else if(_materialFileNameArray.Num() > 0){
			_FileName = _materialFileNameArray[0];
			_FilePath = _materialFilePathArray[0];
			_FileDownUrl = _materialFileDownUrlArray[0];
			_FileId = _materialFileIDArray[0];

			print(TEXT("fileDownUrl is : %s"), *_FileDownUrl);

			_materialFileNameArray.RemoveAt(0);
			_materialFilePathArray.RemoveAt(0);
			_materialFileDownUrlArray.RemoveAt(0);
			_materialFileIDArray.RemoveAt(0);

			FString filePathId = FPaths::ProjectContentDir() + _FilePath + "/" + _FileId;
			FString filePathName = FPaths::ProjectContentDir() + _FilePath + "/" + _FileName + ".uasset";

			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			if (PlatformFile.FileExists(*filePathName) && PlatformFile.FileExists(*filePathId))
			{
				numSize--;
				numLoading += simModelNum;
				textLoading = FString::FromInt(int32(numLoading)) + " %";
				EventChange();
				CleckFile();
			}
			else
			{
				downloadMesh(_FileDownUrl);
			}
		}
		else
		{
			numLoading = 100.0f;
			textLoading = FString::FromInt(int32(numLoading)) + " %";
			EventChange();
			openMap();
		}
	}
	else {
		numLoading = 100.0f;
		textLoading = FString::FromInt(int32(numLoading)) + " %";
		EventChange();
		openMap();
	}

}

void UCUI_loading::loadROOMTYPEDetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += ROOMTYPE_INFO_INTERFACE;
	url += gid;

	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_loading::LoadROOMTYPEDetailCompleteCallback);
	HttpRequest->ProcessRequest();
}

void UCUI_loading::LoadROOMTYPEDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";
	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		MessageBody = Response->GetContentAsString();
		Document jsonDoc;
		jsonDoc.Parse(MessageBody);
		if (jsonDoc["code"].GetInt() == 1000)
		{
			_mapFileName = jsonDoc["data"]["fileName"].GetFString();
			_mapFilePath = jsonDoc["data"]["folderPath"].GetFString();
			_mapFileDownUrl = jsonDoc["data"]["fileUrl"].GetFString();

			_FileName = _mapFileName;
			_FilePath = _mapFilePath;
			_FileDownUrl = _mapFileDownUrl;

			// 验证模型 版本号  201941
			_FileId =FString::FromInt(jsonDoc["data"]["fileVersion"].GetInt());
			FString filePathId = FPaths::ProjectContentDir() + _FilePath + "/" + _FileId;

			FString filePathName = FPaths::ProjectContentDir() + _FilePath + "/" + _FileName + ".umap";

			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			// 201941
			if (PlatformFile.FileExists(*filePathName) && PlatformFile.FileExists(*filePathId))
			{
				/************************************************************************/
				/* 继续执行代码                                                         */
				/************************************************************************/
				numSize -= 20;
				numLoading = simModelNum * 20;
				textLoading = FString::FromInt(int32(numLoading)) + " %";
				EventChange();
				StartModeData();
			}
			else
			{
				downloadMesh(_FileDownUrl);
			}
		}

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
}
void UCUI_loading::downloadMesh(FString url)
{
	// print(TEXT("downloadMesh"));
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	HttpRequest->SetURL(url);
	HttpRequest->SetVerb(TEXT("GET"));
	prevNum = 0;
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_loading::downloadMeshComplete);
	HttpRequest->OnRequestProgress().BindUObject(this, &UCUI_loading::downloadRequestProgress);
	HttpRequest->ProcessRequest();
}

void UCUI_loading::downloadMeshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// print(TEXT("downloadMeshComplete"));
	prevNum = 0;
	if (Response.IsValid())
	{
		if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) {

			const int32 count = Response->GetContentLength();
			TArray<uint8> tdata = Response->GetContent();
			FString url = "";

			// 设置文件路径
			if (isJsonFile)
			{
				url = FPaths::ProjectContentDir() + "/ScenarioConfigurationFile.json";
			}
			else
			{
				if (isMapFile)
				{
					url = FPaths::ProjectContentDir() + _FilePath + "/map.zip";
				}
				else
				{
					url = FPaths::ProjectContentDir() + _FilePath + "/mesh.zip";
				}
				// 创建目录
				VerifyOrCreateDirectory(FPaths::ProjectContentDir() + _FilePath);
				print(TEXT("%s"), *url);
			}

			FString pathId = FPaths::ProjectContentDir() + _FilePath + "/" + _FileId;

			FFileHelper::SaveStringToFile(_FileId, *pathId);
			// 保存文件
			Tools::saveDataToFile(url, tdata);

			if (!isJsonFile)
			{
				FString cmd = "7z.exe x " + url + " -o\"" + FPaths::ProjectContentDir() + _FilePath;
				// print(TEXT("cmd:%s"), *cmd);
				Tools::system_hide(cmd.GetCharArray().GetData());
				if (isMapFile)
				{
					/************************************************************************/
					/*                                                                      */
					/************************************************************************/
					numSize -= 20;
					isMapFile = false;
					StartModeData();
				}
				else
				{
					/************************************************************************/
					/*                                                                      */
					/************************************************************************/
					numSize--;
					CleckFile();
				}
			}
			else
			{
				FString result;
				FFileHelper::LoadFileToString(result, *url);
				Document jsonDoc;
				jsonDoc.Parse(result);
				int32 len = int32(jsonDoc["model"].GetArray().Size());
				for (int32 i = 0; i < len; i++)
				{
					_modelGidArray.Push(jsonDoc["model"][i]["gid"].GetFString());
				}
				len = int32(jsonDoc["material"].GetArray().Size());
				for (int32 i = 0; i < len; i++)
				{
					_materialGidArray.Push(jsonDoc["material"][i]["gid"].GetFString());
				}
// 				print(TEXT("_modelGidArray"));
// 				for (int32 i = 0; i < _modelGidArray.Num(); i++) {
// 					print(TEXT("%s"), *_modelGidArray[i]);
// 				}
// 				print(TEXT("_materialGidArray"));
// 				for (int32 i = 0; i < _materialGidArray.Num(); i++) {
// 					print(TEXT("%s"), *_materialGidArray[i]);
// 				}
				// isJsonFile = false;
				CalculatedRatio();
				
			}
			
		}
	}
	else
	{
		gidScheme = "1";
		CalculatedRatio();
	}
}

void UCUI_loading::downloadRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSend, int32 InBytesReceived)
{
	FHttpResponsePtr HttpResponse = HttpRequest->GetResponse();

	if (HttpResponse.IsValid())
	{
		int32 TotalSize = HttpResponse->GetContentLength();
		float parent = (float)InBytesReceived / (float)TotalSize;
		// print(TEXT("%.2f"), parent);
		// loadingNum = 120.0f * parent;
		// eventloading();
		if (isMapFile)
		{
			numLoading = simModelNum * 20 * parent;
		}
		else 
		{
			numLoading = numLoading + simModelNum * parent - prevNum;
			prevNum = simModelNum * parent;
		}

		textLoading = FString::FromInt(int32(numLoading)) + " %";
		EventChange();
	}
}

bool UCUI_loading::VerifyOrCreateDirectory(const FString& TestDir)
{
	print(TEXT("TestDir:%s"), *TestDir);
	TArray<FString> dirArr;
	TestDir.ParseIntoArray(dirArr, TEXT("/"), false);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString tmpDir = "";
	for (int32 i = 0; i < dirArr.Num(); ++i)
	{
		if (i == 0)
		{
			tmpDir += dirArr[i];
		}
		else
		{
			tmpDir = tmpDir + "/" + dirArr[i];
		}
		if (!PlatformFile.DirectoryExists(*tmpDir))
		{
			if (!PlatformFile.CreateDirectory(*tmpDir))
			{
				print(TEXT("aaa: %s"), *tmpDir);
			}
			else
			{
				print(TEXT("bbb: %s"), *tmpDir);
			}
			// print(TEXT("tmpDir:%s"), *tmpDir);

			if (!PlatformFile.DirectoryExists(*tmpDir))
			{
			}
		}
	}
	return true;
}


