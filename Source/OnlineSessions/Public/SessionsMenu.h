// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionsMenu.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuSetupCompleted, bool, bWasSuccessful);

UCLASS()
class ONLINESESSIONS_API USessionsMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetupMenu(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/...")));

protected:

	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	UFUNCTION(BlueprintNativeEvent)
	void OnCreateSession(bool bWasSuccessful);

	UFUNCTION(BlueprintNativeEvent)
	void OnFindSessions(const TArray<FSessionSearchResult>& SearchResults, bool bWasSuccessful);

	UFUNCTION(BlueprintNativeEvent)
	void OnJoinSession(EOnlineJoinSessionCompleteResult Result);

	UFUNCTION(BlueprintNativeEvent)
	void OnDestroySession(bool bWasSuccessful);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnStartSession(bool bWasSuccessful);

	UPROPERTY(BlueprintAssignable, Category = "Menu")
	FOnMenuSetupCompleted OnMenuSetupCompleted;

private:

	int32 NumPublicConnections{4};
	FString MatchType{TEXT("FreeForAll")};
	FString PathToLobby{ TEXT("")};
	class UOnlineSessionsSubsystem* OnlineSessionsSubsystem;

	void MenuTearDown();
};