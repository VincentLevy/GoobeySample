// Property of The Goobey Gang LLC

#pragma once

#include "CoreMinimal.h"
#include "AbstractCharacter.h"
#include "Mondo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMoveBlockDelegate, FVector, Direction, float, Value);

/**
 * Class for Mondo
 * Mainly just broadcasts his location and his pushes
 */
UCLASS()
class GOOBEY_2_API AMondo : public AAbstractCharacter
{
	GENERATED_BODY()

protected:
	bool bCanPush;

	AMondo();
	virtual void MoveForward(float Value) override;
	virtual void MoveRight(float Value) override;
	void BroadCastDirection(FVector Direction, float Value) const;
	
	virtual void CharacterAction_Implementation() override;
	virtual void ReleaseCharacterAction_Implementation() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Character | Mondo")
	void MondoAction();

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FMoveBlockDelegate OnPushBlock;
	
};
