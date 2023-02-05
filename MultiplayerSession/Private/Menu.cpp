#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h"

#include "../../../../../Source/Slash/DebugMacros.h"

void UMenu::MenuSetup(int32 NumberOfPublicConnections, FString TypeOfMatch)
{
	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	TObjectPtr<UWorld> World = GetWorld();
	if (World) {

		TObjectPtr<APlayerController> PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {

			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	TObjectPtr<UGameInstance> GameInstance = GetGameInstance();
	if (GameInstance) {
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}

	if (MultiplayerSessionSubsystem) {
		MultiplayerSessionSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		MultiplayerSessionSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		MultiplayerSessionSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		MultiplayerSessionSubsystem->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		MultiplayerSessionSubsystem->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
	}
}

bool UMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (HostButton) HostButton->OnClicked.AddDynamic(this, &UMenu::HostButtonClicked);
	if (JoinButton) JoinButton->OnClicked.AddDynamic(this, &UMenu::JoinButtonClicked);

	return true;
}

void UMenu::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}

void UMenu::OnCreateSession(bool bWasSuccesfull)
{
	if (bWasSuccesfull) {

		SCREEN_WARN_MSG("Session Created Successfully");

		TObjectPtr<UWorld> World = GetWorld();
		if (World) 	World->ServerTravel("/Game/Maps/Lobby?listen");
	}
	else SCREEN_ERR_MSG("Failed to Create Session");

}

void UMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful)
{
}

void UMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
}

void UMenu::OnDestroySession(bool bWasSuccesfull)
{
}

void UMenu::OnStartSession(bool bWasSuccesfull)
{
}

void UMenu::HostButtonClicked()
{
	if (MultiplayerSessionSubsystem) MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);
}

void UMenu::JoinButtonClicked()
{
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	TObjectPtr<UWorld> World = GetWorld();
	if (World) {
		TObjectPtr<APlayerController> PC = World->GetFirstPlayerController();
		if (PC) {
			FInputModeGameOnly InputModeData;
			PC->SetInputMode(InputModeData);
			PC->SetShowMouseCursor(false);
		}
	}
}
