// Property of The Goobey Gang LLC


#include "GoobeyPlayerController.h"

void AGoobeyPlayerController::SetControlRotationAndBroadcast(FRotator NewRotation)
{
	SetControlRotation(NewRotation);
	OnControlTurn.Broadcast(NewRotation);
}