// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "OnlineSessionsSubsystem.generated.h"

UENUM(BlueprintType, Category = "Online Sessions")
enum class EOnlineJoinSessionCompleteResult : uint8
{
	Success,
	SessionIsFull,
	SessionDoesNotExist,
	CouldNotRetrieveAddress,
	AlreadyInSession,
	UnknownError
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnlineOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnlineOnFindSessionsComplete, const TArray<FSessionSearchResult>&, SearchResult, bool, bWasSuccessful);
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
	UFUNCTION(BlueprintCallable)
	void CreateSession(int32 NumPublicConnections, FString MatchType);
	UFUNCTION(BlueprintCallable)
	void FindSessions(int32 MaxSearchResults);
	UFUNCTION(BlueprintCallable)
	void JoinSession(const FSessionSearchResult& SessionResult);
	UFUNCTION(BlueprintCallable)
	void DestroySession();
	UFUNCTION(BlueprintCallable)
	void StartSession();
	UFUNCTION(BlueprintCallable)
	TArray<FSessionSearchResult> GetLastSessionSearchResult();

// Delegates for binding
	UPROPERTY(BlueprintAssignable, Category = "Online Session")
	FOnlineOnCreateSessionComplete OnlineOnCreateSessionComplete;
	UPROPERTY(BlueprintAssignable, Category = "Online Session")
	FOnlineOnFindSessionsComplete OnlineOnFindSessionsComplete;
	UPROPERTY(BlueprintAssignable, Category = "Online Session")
	FOnlineOnDestroySessionComplete OnlineOnDestroySessionComplete;
	UPROPERTY(BlueprintAssignable, Category = "Online Session")
	FOnlineOnStartSessionComplete OnlineOnStartSessionComplete;
	UPROPERTY(BlueprintAssignable, Category = "Online Session")
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

	bool bCreateSessionOnDestroy{ false };
	int32 LastNumPublicConnections;
	FString LastMatchType;
};
