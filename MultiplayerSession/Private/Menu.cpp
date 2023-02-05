#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"

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

void UMenu::HostButtonClicked()
{
	if (MultiplayerSessionSubsystem) {
		MultiplayerSessionSubsystem->CreateSession(NumPublicConnections, MatchType);

		TObjectPtr<UWorld> World = GetWorld();
		if (World) {
			World->ServerTravel("/Game/Maps/Lobby?listen");
		}
	}
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
			FInputModeGameAndUI InputModeData;
			PC->SetInputMode(InputModeData);
			PC->SetShowMouseCursor(false);
		}
	}
}
