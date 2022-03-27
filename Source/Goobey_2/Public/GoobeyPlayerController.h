// Property of The Goobey Gang LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GoobeyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnDelegate, FRotator, NewRotation);

/**
 * 
 */
UCLASS()
class GOOBEY_2_API AGoobeyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetControlRotationAndBroadcast(FRotator NewRotation);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTurnDelegate OnControlTurn;
};
