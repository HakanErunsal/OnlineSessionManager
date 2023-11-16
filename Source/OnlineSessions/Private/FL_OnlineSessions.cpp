// Fill out your copyright notice in the Description page of Project Settings.


#include "FL_OnlineSessions.h"

const TArray<FSessionSearchResultInfo> UFL_OnlineSessions::GetSearchResultsInfo(
	const TArray<FSessionSearchResult>& SearchResults)
{
	TArray<FSessionSearchResultInfo> SearchResultInfos;
	
	Algo::Transform(SearchResults, SearchResultInfos,[] (const FSessionSearchResult& SearchResult)
	{
		return FSessionSearchResultInfo(SearchResult);
	} );
	return SearchResultInfos;
}
