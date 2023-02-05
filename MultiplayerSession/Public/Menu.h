#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

UCLASS()
class MULTIPLAYERSESSION_API UMenu : public UUserWidget
{
	GENERATED_BODY()
	
	public:	

		UFUNCTION(BlueprintCallable)
		void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")));

	protected:

		virtual bool Initialize() override;
		virtual void NativeDestruct() override;

	private:
		
		UPROPERTY(meta = (BindWidget))
		TObjectPtr<class UButton> HostButton;
		UPROPERTY(meta = (BindWidget))
		TObjectPtr<UButton> JoinButton;

		UFUNCTION()
		void HostButtonClicked();

		UFUNCTION()
		void JoinButtonClicked();	
		
		UFUNCTION()
		void MenuTearDown();

		// The subsystem designed to handle all online session functionality
		TObjectPtr<class UMultiplayerSessionSubsystem> MultiplayerSessionSubsystem;

		int32 NumPublicConnections{4};
		FString MatchType{TEXT("FreeForAll")};

};
