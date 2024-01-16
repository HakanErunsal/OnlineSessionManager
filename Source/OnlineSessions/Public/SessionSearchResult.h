#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "SessionSearchResult.generated.h"

	USTRUCT(BlueprintType)
	struct FSessionSearchResult
	{
		GENERATED_USTRUCT_BODY()

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString OwningUserName = "Invalid";
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SessionName = "Invalid";
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumOpenPrivateConnections = 0;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 NumOpenPublicConnections = 0;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PingInMs = 0;
		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsValid = false;
		
		FOnlineSessionSearchResult SearchResult;
	
		FSessionSearchResult(){}
		FSessionSearchResult(const FOnlineSessionSearchResult& Result):
			OwningUserName(Result.Session.OwningUserName),
			NumOpenPrivateConnections(Result.Session.NumOpenPrivateConnections),
			NumOpenPublicConnections(Result.Session.NumOpenPublicConnections),
			PingInMs(Result.PingInMs),
			IsValid(Result.IsValid()),
			SearchResult(Result)
		{}
	};