#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"

	USTRUCT(BlueprintType)
	struct FNewSessionSearchResult
	{
		GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere)
		FString OwningUserName = "Invalid";
		UPROPERTY(EditAnywhere)
		int32 NumOpenPrivateConnections = 0;
		UPROPERTY(EditAnywhere)
		int32 NumOpenPublicConnections = 0;
		UPROPERTY(EditAnywhere)
		int32 PingInMs = 0;
		UPROPERTY(EditAnywhere)
		bool IsValid = false;
		
		FOnlineSessionSearchResult SearchResult;
	
		FNewSessionSearchResult(){}
		FNewSessionSearchResult(const FOnlineSessionSearchResult& Result):
			OwningUserName(Result.Session.OwningUserName),
			NumOpenPrivateConnections(Result.Session.NumOpenPrivateConnections),
			NumOpenPublicConnections(Result.Session.NumOpenPublicConnections),
			PingInMs(Result.PingInMs),
			IsValid(Result.IsValid()),
			SearchResult(Result)
		{}
	};