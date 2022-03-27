// Property of The Goobey Gang LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BouncyPlatform.generated.h"

class UBoxComponent;
class UAudioComponent;


UCLASS()
class GOOBEY_2_API ABouncyPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABouncyPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Launch")
	float LaunchForce;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UAudioComponent* BounceSound;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
