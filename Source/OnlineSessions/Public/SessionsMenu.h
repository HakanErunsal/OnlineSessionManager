// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionsMenu.generated.h"

/**
 * 
 */
UCLASS()
class ONLINESESSIONS_API USessionsMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetupMenu(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")));

protected:

	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);

	//Not Compatible with blueprints??
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);

	UFUNCTION()
	void OnJoinSession(EOnlineJoinSessionCompleteResult Result);

	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private:

	int32 NumPublicConnections{4};
	FString MatchType{TEXT("FreeForAll")};
	
	class UOnlineSessionsSubsystem* OnlineSessionsSubsystem;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Host;

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Join;

	UFUNCTION()
	void HostButtonClick();
	
	UFUNCTION()
	void JoinButtonClick();

	void MenuTearDown();
};