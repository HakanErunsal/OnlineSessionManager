// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionsMenu.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionsSubsystem.h"
#include "SessionSearchResult.h"
#include "Components/Button.h"

void USessionsMenu::SetupMenu(int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"),*LobbyPath);
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if(GameInstance)
	{
		OnlineSessionsSubsystem = GameInstance->GetSubsystem<UOnlineSessionsSubsystem>();
	}

	if (OnlineSessionsSubsystem)
	{
		OnlineSessionsSubsystem->OnlineOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		OnlineSessionsSubsystem->OnlineOnFindSessionsComplete.AddDynamic(this, &ThisClass::OnFindSessions);
		OnlineSessionsSubsystem->OnlineOnJoinSessionComplete.AddDynamic(this, &ThisClass::OnJoinSession);
		OnlineSessionsSubsystem->OnlineOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);	
		OnlineSessionsSubsystem->OnlineOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}

	OnMenuSetupCompleted.Broadcast(true);
}

bool USessionsMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	return true;
}


void USessionsMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}


void USessionsMenu::OnCreateSession_Implementation(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Failed to create a session!")));
		}
	}
}

void USessionsMenu::OnFindSessions_Implementation(const TArray<FSessionSearchResult>& SearchResults, bool bWasSuccessful)
{
	/*if (OnlineSessionsSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : SearchResults)
	{
		FString SettingsValue;
		Result.SearchResult.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		
		if(SettingsValue == MatchType)
		{
			//OnlineSessionsSubsystem->JoinSession(Result);
			return;
		}
	}*/
}

void USessionsMenu::OnJoinSession_Implementation(EOnlineJoinSessionCompleteResult Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
			
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void USessionsMenu::OnDestroySession_Implementation(bool bWasSuccessful)
{
}

void USessionsMenu::OnStartSession_Implementation(bool bWasSuccessful)
{
}


void USessionsMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if(World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if(PlayerController)
		{
			const FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}
}