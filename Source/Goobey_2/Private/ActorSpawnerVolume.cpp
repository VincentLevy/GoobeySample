// Property of The Goobey Gang LLC


#include "ActorSpawnerVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
AActorSpawnerVolume::AActorSpawnerVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
}

// Called when the game starts or when spawned
void AActorSpawnerVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AActorSpawnerVolume::OnBoxBeginOverlap);

	SetHideActors(true);
}

void AActorSpawnerVolume::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetHideActors(false);
}

void AActorSpawnerVolume::SetHideActors(const bool HiddenValue)
{

	if(ActorsToSpawn.Num() <= 0)
	{
		return;
	}
	
	for (auto Actor : ActorsToSpawn)
	{

		if(Actor)
		{
			Actor->SetActorHiddenInGame(HiddenValue);

			if(HiddenValue)
			{
				Actor->SetActorEnableCollision(false);
			}
			else
			{
				Actor->SetActorEnableCollision(true);
			}
		}
		
	}
}

// Called every frame
// void AActorSpawnerVolume::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

