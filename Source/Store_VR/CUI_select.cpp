// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_select.h"
#include "CameraPlayerController.h"
#include "CameraGameInstance.h"
#include "Store_VRGameModeBase.h"
#include "NetworkInterface.h"
#include "rapidjson/document.h"
using namespace rapidjson;


#define print(Format, ...) { UE_LOG(LogClass, Warning, Format, ##__VA_ARGS__); }
void UCUI_select::initSelect()
{
	if (GetWorld())
	{
		CameraPlayerController = Cast<ACameraPlayerController>(GetWorld()->GetFirstPlayerController());
		gameMode = Cast<AStore_VRGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	pageNum = 0;
	pageMax = 0;

	backgroundImage = CameraPlayerController->GetTexture("Image/select_background.png");

	if (backgroundImage)
	{
		UpdataBGImage();
	}


	EventArrayUIImage();
	
	searchKeyword = "";
	eHouse = EHouseType::House;
	bSearch = false;
	pageNum = 1;
	loadHOUSETYPELISTSchemeDetail();
}

void UCUI_select::HouseNetCall()
{
	searchKeyword = "";
	eHouse = EHouseType::House;
	bSearch = false;
	pageNum = 0;
	btnDown();
}

void UCUI_select::DesignNetCall()
{
	searchKeyword = "";
	eHouse = EHouseType::Design;
	bSearch = false;
	pageNum = 0;
	btnDown();
}

void UCUI_select::btnUp()
{
	pageNum--;

	DisplayConfiguration();
}

void UCUI_select::btnDown()
{
	pageNum++;

	DisplayConfiguration();
}

void UCUI_select::DisplayConfiguration()
{
	/************************************************************************/
	/* 调用网络接口获取10个以内的户型                                       */
	/* 获取数据总长/每页数量, 得到 pageMax									*/
	/* 是否显示上一页下一页按钮												*/
	/* 此方法加入网络功能后, 改动较大										*/			
	/************************************************************************/
	if (eHouse == EHouseType::House) {
		loadHOUSETYPELISTSchemeDetail();
	}
	else {
		if (!searchKeyword.Equals(""))
		{
			EventIsNumPhoto();
		}
		else {
			loadSCHEMELISTSchemeDetail(false);
		}
	}
	
	
}

void UCUI_select::submitSearch(FText submitText)
{
	searchKeyword = submitText.ToString();
	bSearch = true;
	pageNum = 0;
	btnDown();
}

void UCUI_select::changeTextBox(FText btnName)
{
	textName = btnName;
	EventchangeTextBox();
}

void UCUI_select::searchInit()
{
	/************************************************************************/
	/* 通过网络获取热门搜索数量及数据                                       */
	/************************************************************************/
	searchNum = 5;

	searchDataArray.Empty();

	for (int32 i = 0; i < 5; i++)
	{
		FSearchData searchData;
		searchData.searchBtn = "btn name is : " + FString::FromInt(i);
		searchData.url = "btn url is : " + FString::FromInt(i);
		searchDataArray.Push(searchData);
	}

	RefreshSearch();
}

void UCUI_select::ChangeInst()
{
	if (GetWorld())
	{
		UCameraGameInstance* CGInstance = Cast<UCameraGameInstance>(GetWorld()->GetGameInstance());
		if (CGInstance != nullptr)
		{
			CGInstance->emap = EMaps::Index;
		}
	}
}

void UCUI_select::loadHOUSETYPELISTSchemeDetail()
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += HOUSETYPE_LIST_INTERFACE;

	HttpRequest->SetURL(url);

	TMap<FString, FString> params;
	params.Add(TEXT("curPage"), FString::FromInt(pageNum));
	params.Add(TEXT("pageSize"), FString::FromInt(10));
	HttpRequest->AppendToHeader(TEXT("curPage"), FString::FromInt(pageNum));
	HttpRequest->AppendToHeader(TEXT("pageSize"), FString::FromInt(10));

//	if (!searchKeyword.Equals(""))
//	{
		// url = url + "&propertyName=" + searchKeyword;
	params.Add(TEXT("propertyName"), searchKeyword);
//	HttpRequest->AppendToHeader(TEXT("propertyName"), searchKeyword);
//	}

	FString paramsString = getContentWithMap(params);

	// FString p = TEXT("params");

	HttpRequest->SetContentAsString(paramsString);

	log(paramsString);

	// searchKeyword 搜索关键字

	HttpRequest->SetVerb(TEXT("POST"));

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_select::LoadHOUSETYPELISTSchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_select::LoadHOUSETYPELISTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		log(MessageBody);
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		//成功获取网络返回的数据
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			isSearchData = !jsonDoc["isSearchData"].GetBool();
			dataMax = jsonDoc["total"].GetInt();

			// 计算总页数
			if (dataMax > 10)
			{
				if (dataMax % 10 == 0)
				{
					pageMax = dataMax / 10;
				}
				else
				{
					pageMax = dataMax / 10 + 1;
				}
			}
			else
			{
				pageMax = 1;
			}

			// 是否显示两个按钮
			if (pageNum == 1)
			{
				isShowUpBtn = false;
			}
			else
			{
				isShowUpBtn = true;
			}

			if (pageNum == pageMax)
			{
				isShowNetBtn = false;
			}
			else
			{
				isShowNetBtn = true;
			}

			// 当前页面显示数据数量
			if (pageNum == pageMax) {
				if (dataMax % 10 == 0)
				{
					pageNowNum = 10;
				}
				else
				{
					pageNowNum = dataMax % 10;
				}
			}
			else {
				pageNowNum = 10;
			}

// 			// 判断是否搜索数据为空
// 			if (bSearch && dataMax == 0)
// 			{
// 				// 获取热门推荐数据 最多 5 条
// 				// 模拟数据
// 				dataMax = 5;
// 				isShowUpBtn = false;
// 				isShowNetBtn = false;
// 				pageMax = 1;
// 				pageNum = 1;
// 				pageNowNum = dataMax;
// 
// 				isSearchData = true;
// 			}
// 			else
// 			{
// 				isSearchData = false;
// 			}

			HouseGidArray.Empty();
			HouseNameArray.Empty();
			PropertyNameArray.Empty();
			RestaurantNumArray.Empty();
			RoomNumArray.Empty();
			OutAreaArray.Empty();
			InAreaArray.Empty();
			ImageUrlArray.Empty();
			houseTypeArray.Empty();

			if (!dataMax == 0)
			{
				for (int32 i = 0; i < pageNowNum; i++)
				{
					HouseGidArray.Push(FString::FromInt(jsonDoc["data"][i]["gid"].GetInt()));
					HouseNameArray.Push(jsonDoc["data"][i]["houseTypeName"].GetFString());
					PropertyNameArray.Push(jsonDoc["data"][i]["propertyName"].GetFString());
					RoomNumArray.Push(jsonDoc["data"][i]["roomNum"].GetInt());
					RestaurantNumArray.Push(jsonDoc["data"][i]["restaurantNum"].GetInt());
					OutAreaArray.Push(jsonDoc["data"][i]["builtUpArea"].GetFString());
					InAreaArray.Push(jsonDoc["data"][i]["inJacketArea"].GetFString());
					ImageUrlArray.Push(jsonDoc["data"][i]["imageUrl"].GetFString());

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

					for (int32 j = 0; j < int32(jsonDoc["data"][i]["roomTypeInfoList"].GetArray().Size()); j++)
					{
						int32 num = jsonDoc["data"][i]["roomTypeInfoList"][j]["configStyleGid"].GetInt();
						int32 gid = jsonDoc["data"][i]["roomTypeInfoList"][j]["gid"].GetInt();
						int32 schemeGid = jsonDoc["data"][i]["roomTypeInfoList"][j]["defaultSchemeGid"].GetInt();
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
					}
					houseTypeArray.Push(houseType);
				}
			}

			
		}
		RefreshPage();

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		log(MessageBody);
		print(TEXT("gameMode->CreateNetErrorUI() : %s"), *MessageBody);
		gameMode->CreateNetErrorUI();
	}
}

void UCUI_select::log(FString myString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, myString);
	}
}

void UCUI_select::loadSCHEMELISTSchemeDetail(bool bNumPhoto)
{
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
	FString url = INTERFACE_ADDRESS;
	url += SCHEME_LIST_INTERFACE;

 	TMap<FString, FString> params;
 	params.Add(TEXT("curPage"), FString::FromInt(pageNum));
 	params.Add(TEXT("pageSize"), FString::FromInt(10));

	if (!searchKeyword.Equals(""))
	{
		if (bNumPhoto)
		{
			params.Add(TEXT("userPhone"), searchKeyword);
		}
		else
		{
			params.Add(TEXT("schemeName"), searchKeyword);
		}
	}

	HttpRequest->SetURL(url);

	FString paramsString = getContentWithMap(params);

	HttpRequest->SetContentAsString(paramsString);
	HttpRequest->SetVerb(TEXT("POST"));

	log(paramsString);
	print(TEXT("paramsString : %s"), *paramsString);

	//UE4对象访问网络使用 BindObject 方法进行绑定
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UCUI_select::LoadSCHEMELISTSchemeDetailCompleteCallback);

	HttpRequest->ProcessRequest();
}

void UCUI_select::LoadSCHEMELISTSchemeDetailCompleteCallback(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FString MessageBody = "";

	if (!searchKeyword.Equals(""))
	{
		print(TEXT("SCHEME_LIST_INTERFACE : 1"));
	}

	if (!Response.IsValid())
	{
		MessageBody = "{\"success\":\"Error: Unable to process HTTP Request!\"}";
		gameMode->CreateNetErrorUI();
	}
	else if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		if (!searchKeyword.Equals(""))
		{
			print(TEXT("SCHEME_LIST_INTERFACE : 2"));
		}
		//成功获取网络返回的数据
		MessageBody = Response->GetContentAsString();

		Document jsonDoc;
		jsonDoc.Parse(MessageBody);

		if (jsonDoc["code"].GetInt() == 1000)
		{
			if (!searchKeyword.Equals(""))
			{
				print(TEXT("SCHEME_LIST_INTERFACE : 3"));
			}

			isSearchData = !jsonDoc["isSearchData"].GetBool();
			dataMax = jsonDoc["total"].GetInt();

			if (!searchKeyword.Equals(""))
			{
				print(TEXT("SCHEME_LIST_INTERFACE : 4"));
			}
			// 计算总页数
			if (dataMax > 10)
			{
				if (dataMax % 10 == 0)
				{
					pageMax = dataMax / 10;
				}
				else
				{
					pageMax = dataMax / 10 + 1;
				}
			}
			else
			{
				pageMax = 1;
			}

			// 是否显示两个按钮
			if (pageNum == 1)
			{
				isShowUpBtn = false;
			}
			else
			{
				isShowUpBtn = true;
			}

			if (pageNum == pageMax)
			{
				isShowNetBtn = false;
			}
			else
			{
				isShowNetBtn = true;
			}

			// 当前页面显示数据数量
			if (pageNum == pageMax) {
				if (dataMax % 10 == 0)
				{
					pageNowNum = 10;
				}
				else
				{
					pageNowNum = dataMax % 10;
				}
			}
			else {
				pageNowNum = 10;
			}

			HouseGidArray.Empty();
			HouseNameArray.Empty();
			PropertyNameArray.Empty();
			RestaurantNumArray.Empty();
			RoomNumArray.Empty();
			OutAreaArray.Empty();
			InAreaArray.Empty();
			ImageUrlArray.Empty();
			houseTypeArray.Empty();
			gidArray.Empty();
			gidSchemeArray.Empty();
			HouseStyleArray.Empty();
			phoneNumArray.Empty();

			if (!searchKeyword.Equals(""))
			{
				print(TEXT("SCHEME_LIST_INTERFACE : 5"));
			}

			if (!dataMax == 0)
			{
				for (int32 i = 0; i < pageNowNum; i++)
				{
					HouseGidArray.Push(FString::FromInt(jsonDoc["data"][i]["houseTypeInfo"]["gid"].GetInt()));
					HouseNameArray.Push(jsonDoc["data"][i]["houseTypeInfo"]["houseTypeName"].GetFString());
					PropertyNameArray.Push(jsonDoc["data"][i]["houseTypeInfo"]["propertyName"].GetFString());

					RestaurantNumArray.Push(jsonDoc["data"][i]["roomTypeInfo"]["restaurantNum"].GetInt());
					RoomNumArray.Push(jsonDoc["data"][i]["roomTypeInfo"]["roomNum"].GetInt());

					OutAreaArray.Push(jsonDoc["data"][i]["houseTypeInfo"]["builtUpArea"].GetFString());
					InAreaArray.Push(jsonDoc["data"][i]["houseTypeInfo"]["inJacketArea"].GetFString());
					ImageUrlArray.Push(jsonDoc["data"][i]["showImgUrl"].GetFString());

					gidArray.Push(FString::FromInt(jsonDoc["data"][i]["roomTypeInfoGid"].GetInt()));
					gidSchemeArray.Push(FString::FromInt(jsonDoc["data"][i]["gid"].GetInt()));

					HouseStyleArray.Push(jsonDoc["data"][i]["roomTypeInfo"]["configStyleName"].GetFString());

					phoneNumArray.Push(jsonDoc["data"][i]["userPhone"].GetFString());
				}
			}
		}

		if (!searchKeyword.Equals(""))
		{
			print(TEXT("SCHEME_LIST_INTERFACE : 6"));
		}

		RefreshPage();

	}
	else
	{
		MessageBody = FString::Printf(TEXT("{\"success\":\"HTTP Error: %d\"}"), Response->GetResponseCode());
		gameMode->CreateNetErrorUI();
	}
}


FString UCUI_select::getContentWithMap(TMap<FString, FString> params) {
	FString paramsString = "";
	for (TPair<FString, FString>& element : params)
	{
		paramsString += element.Key;
		paramsString += "=";
		paramsString += *element.Value;
		paramsString += "&";
	}
	paramsString.RemoveAt(paramsString.Len() - 1);
	return paramsString;
}