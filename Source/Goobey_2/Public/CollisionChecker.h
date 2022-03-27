// Property of The Goobey Gang LLC

#pragma once

#include "CoreMinimal.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "CollisionChecker.generated.h"

/*
 * Class use to determine if an actor can spawn in a certain spot or not
 * Used to perform collision checks
 */
UCLASS()
class GOOBEY_2_API ACollisionChecker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollisionChecker();
	
	// virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	// virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Capsule)
	UCapsuleComponent* CapsuleComponent;
};
