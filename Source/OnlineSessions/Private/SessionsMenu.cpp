// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionsMenu.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionsSubsystem.h"
#include "Components/Button.h"

void USessionsMenu::SetupMenu(int32 NumberOfPublicConnections, FString TypeOfMatch)
{
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
		OnlineSessionsSubsystem->OnlineOnFindSessionsComplete.AddDynamic(this, &ThisClass::OnFindSession);
		OnlineSessionsSubsystem->OnlineOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		OnlineSessionsSubsystem->OnlineOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);	
		OnlineSessionsSubsystem->OnlineOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool USessionsMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if(Button_Host)
	{
		Button_Host->OnClicked.AddDynamic(this, &USessionsMenu::HostButtonClick);
	}

	if(Button_Join)
	{
		Button_Join->OnClicked.AddDynamic(this, &USessionsMenu::JoinButtonClick);
	}
	
	return true;
}


void USessionsMenu::NativeDestruct()
{
	MenuTearDown();
	Super::NativeDestruct();
}

void USessionsMenu::OnCreateSession(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("Session Created Successfully!"))
				);
		}

		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel("/Game/ThirdPerson/Maps/M_Lobby?listen");
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
				);
		}
	}
}

void USessionsMenu::OnFindSession(const TArray<FSessionSearchResult>& SearchResults, bool bWasSuccessful)
{
	if(OnlineSessionsSubsystem == nullptr)
	{
		return;
	}
	
	for (auto Result : SearchResults)
	{
		FString SettingsValue;
		Result.SearchResult.Session.SessionSettings.Get(FName("MatchType"), MatchType);
		if(SettingsValue == MatchType)
		{
			OnlineSessionsSubsystem->JoinSession(Result.SearchResult);
			return;
		}
	}
}

void USessionsMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if(Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if(SessionInterface)
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if(PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void USessionsMenu::OnDestroySession(bool bWasSuccessful)
{
}

void USessionsMenu::OnStartSession(bool bWasSuccessful)
{
}

void USessionsMenu::HostButtonClick()
{
	if(OnlineSessionsSubsystem)
	{
		OnlineSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void USessionsMenu::JoinButtonClick()
{
	if(OnlineSessionsSubsystem)
	{
		OnlineSessionsSubsystem->FindSession(16);
	}
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
