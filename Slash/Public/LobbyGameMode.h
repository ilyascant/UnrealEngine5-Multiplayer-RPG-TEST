#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class SLASH_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

	public:
		virtual void PostLogin(APlayerController* NewPlayer) override;
		virtual void Logout(AController* Exiting) override;
};
