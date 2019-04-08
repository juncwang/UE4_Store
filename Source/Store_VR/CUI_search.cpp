// Fill out your copyright notice in the Description page of Project Settings.

#include "CUI_search.h"





void UCUI_search::SetFSearch(FSearchData searchData)
{
	searchBtn = searchData.searchBtn;
	url = searchData.url;

	EventSetText();
}
