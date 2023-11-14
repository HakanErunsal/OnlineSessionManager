// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionsMenu.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionsSubsystem.h"
#include "Components/Button.h"
#include "Kismet/KismetStringLibrary.h"

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
		OnlineSessionsSubsystem->OnlineOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSession);
		OnlineSessionsSubsystem->OnlineOnJoinSessionComplete.AddDynamic(this, &ThisClass::OnJoinSession);
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
		UWorld* World = GetWorld();
		if(World)
		{
			World->ServerTravel("/Game/ThirdPerson/Maps/M_Lobby?listen");
		}
	}
}

void USessionsMenu::OnFindSession(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString::Printf(TEXT("Find session completed with: %s"), *UKismetStringLibrary::Conv_BoolToString(bWasSuccessful))
			);
	}
	
	if(OnlineSessionsSubsystem == nullptr)
	{
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString::Printf(TEXT("Number of found sessions: %d"), SearchResults.Num())
			);
	}

	for (auto Result : SearchResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString::Printf(TEXT("Found session - Owner Name: %s - Match Type: %s"), *Result.Session.OwningUserName, *SettingsValue)
				);
		}
		
		if(SettingsValue == MatchType)
		{
			OnlineSessionsSubsystem->JoinSession(Result);
			return;
		}
	}
}

void USessionsMenu::OnJoinSession(EOnlineJoinSessionCompleteResult Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
			
			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Green,
					FString::Printf(TEXT("Address: %s"), *Address)
					);
			}*/
			
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
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
		OnlineSessionsSubsystem->FindSession(1000);
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
