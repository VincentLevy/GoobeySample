// Property of The Goobey Gang LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MondoPushBlock.generated.h"

class UBoxComponent;

UCLASS()
class GOOBEY_2_API AMondoPushBlock : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMondoPushBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxCollision;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnMondoPushBlock(FVector Direction, float Value);
};
