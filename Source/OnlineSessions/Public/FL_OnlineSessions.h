// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionsSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FL_OnlineSessions.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType, Category = "Online Session Helpers")
struct FSessionSearchResultInfo
{
	GENERATED_BODY()

public:
	/** Owner name of the session */
	UPROPERTY(BlueprintReadOnly)
	FString OwningUserName = "Invalid";
	/** The number of private connections that are available (read only) */
	UPROPERTY(BlueprintReadOnly)
	int32 NumOpenPrivateConnections = 0;
	/** The number of publicly available connections that are available (read only) */
	UPROPERTY(BlueprintReadOnly)
	int32 NumOpenPublicConnections = 0;
	/** Ping value */
	UPROPERTY(BlueprintReadOnly)
	int32 PingInMs = 0;
	/** Is Valid */
	UPROPERTY(BlueprintReadOnly)
	bool IsValid = false;

public:
	FSessionSearchResultInfo() {}
	FSessionSearchResultInfo(const FSessionSearchResult& Result):
		OwningUserName(Result.SearchResult.Session.OwningUserName),
		NumOpenPrivateConnections(Result.SearchResult.Session.NumOpenPrivateConnections),
		NumOpenPublicConnections(Result.SearchResult.Session.NumOpenPublicConnections),
		PingInMs(Result.SearchResult.PingInMs),
		IsValid(Result.SearchResult.IsValid())
	{}
};

UCLASS()
class ONLINESESSIONS_API UFL_OnlineSessions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Helpers")
	static const TArray<FSessionSearchResultInfo> GetSearchResultsInfo(const TArray<FSessionSearchResult>& SearchResults);
};
