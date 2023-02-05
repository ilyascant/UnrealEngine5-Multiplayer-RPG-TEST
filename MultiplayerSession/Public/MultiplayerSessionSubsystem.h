#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MultiplayerSessionSubsystem.generated.h"


UCLASS()
class MULTIPLAYERSESSION_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	public:
		UMultiplayerSessionSubsystem();

		//
		// To handle session functionality. The menu class will call these
		//

		void CreateSession(int32 NumPublicConnections, FString MatchType);
		void FindSessions(int32 MaxSearchResults);
		void JoinSession(const FOnlineSessionSearchResult& SessionResult);
		void DestroySession();
		void StartSession();

	protected:

		//
		// Internal callback for the delegates we'll add to the Online Session Interface delegate list
		// Theese don't need to be called outside this class.
		//

		void OnCreateSessionComplete(FName SessionName, bool bWasSuccesful);
		void OnFindSessionsComplete(bool bWasSuccesful);
		void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
		void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
		void OnStartSessionComplete(FName SessionName, bool bWasSuccessful); 


	private:
		IOnlineSessionPtr SessionInterface;
		TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

		//
		// To add to the Onlie Session Interface delegate list;
		// We'll bind ourr MultiplayerSessionSubsystem internal callback to these.
		//

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
