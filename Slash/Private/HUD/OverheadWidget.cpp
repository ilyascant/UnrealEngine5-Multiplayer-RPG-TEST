#include "HUD/OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText) {
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_MAX:
		break;
	default:
		break;
	}

	SetDisplayText(Role);
}

void UOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	FString PlayerName = InPawn->GetPlayerState()->GetPlayerName();
	SetDisplayText(PlayerName);
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();

	Super::NativeDestruct();
}
