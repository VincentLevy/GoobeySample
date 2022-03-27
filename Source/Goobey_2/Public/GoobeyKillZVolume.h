// Property of The Goobey Gang LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoobeyKillZVolume.generated.h"

class UBoxComponent;

UCLASS()
class GOOBEY_2_API AGoobeyKillZVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoobeyKillZVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* BoxComponent;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

};
