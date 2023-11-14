// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "OnlineSessionsSubsystem.generated.h"

/**
 *  Declaring own custom delegates for the Menu Class to bind callbacks to
 */
USTRUCT(BlueprintType, Category = "Online Sessions")
struct FSessionSearchResult
{
	GENERATED_BODY()

	FOnlineSessionSearchResult SearchResult;
	FSessionSearchResult();
	FSessionSearchResult(const FOnlineSessionSearchResult& Result): SearchResult(Result) {}	
};

UENUM(BlueprintType, Category = "Online Sessions")
enum class EOnlineJoinSessionCompleteResult
{
	Success,
	SessionIsFull,
	SessionDoesNotExist,
	CouldNotRetrieveAddress,
	AlreadyInSession,
	UnknownError
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnlineOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SearchResult, bool bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineOnJoinSessionComplete, EOnlineJoinSessionCompleteResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineOnStartSessionComplete, bool, bWasSuccessful);

UCLASS()
class ONLINESESSIONS_API UOnlineSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	public:
        UOnlineSessionsSubsystem();

//
// To handle session functionality. The Menu class will call these
//
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	void FindSession(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();

// Delegates for binding
	FOnlineOnCreateSessionComplete OnlineOnCreateSessionComplete;
	FOnlineOnFindSessionsComplete OnlineOnFindSessionsComplete;
	FOnlineOnDestroySessionComplete OnlineOnDestroySessionComplete;
	FOnlineOnStartSessionComplete OnlineOnStartSessionComplete;
	FOnlineOnJoinSessionComplete OnlineOnJoinSessionComplete;
	
	protected:

// Internal callbacks for the delegates will be added to the Online Session Interface delegate list
// No need to be called outside of this class

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName,EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	
	private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	// To add to the Online Session Interface delegate list.
	// Will bind OnlineSessionsSubsystem internal callbacks to these
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;
};
