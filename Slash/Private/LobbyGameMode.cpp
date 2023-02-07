#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Slash/DebugMacros.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GameState)
	{
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
		TObjectPtr<APlayerState> PlayerState = NewPlayer->GetPlayerState<APlayerState>();

		SCREEN_CUSTOM_MSG(1, 60.f, FColor::Yellow, "Players in game: %d", NumberOfPlayers);

		if (PlayerState) {
			FString PlayerName = PlayerState->GetPlayerName();
			SCREEN_CUSTOM_MSG(-1, 15.f, FColor::Cyan, "%s has joined the game", *PlayerName);
		}

		if (TObjectPtr<UWorld> World = GetWorld()) {
			bUseSeamlessTravel = true;
			World->ServerTravel(FString(TEXT("/Game/Maps/Test_Level?listen")));
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	TObjectPtr<APlayerState> PlayerState = Exiting->GetPlayerState<APlayerState>();

	SCREEN_CUSTOM_MSG(1, 60.f, FColor::Yellow, "Players in game: %d", NumberOfPlayers - 1);


	if (PlayerState) {
		FString PlayerName = PlayerState->GetPlayerName();
		SCREEN_CUSTOM_MSG(-1, 15.f, FColor::Cyan, "%s has exited the game", *PlayerName);
	}
}
