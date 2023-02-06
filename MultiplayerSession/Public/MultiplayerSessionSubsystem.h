#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MultiplayerSessionSubsystem.generated.h"



//
// Declaring own custom delegates for the Menu class to bind callbacks to
//

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccesfull);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccesfull);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccesfull);


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

		//
		// Own custom delegates for the Menu class to bind callbacks to
		//

		FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
		FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;
		FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
		FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
		FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;


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
		TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

		//
		// To add to the Online Session Interface delegate list;
		// We'll bind our MultiplayerSessionSubsystem internal callback to these.
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

		bool bCreateSessionOnDestroy{ false };
		int32 LastNumPublicConnections;
		FString LastMatchType;
};
