// Property of The Goobey Gang LLC


#include "GoobeyKillZVolume.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h" 

// Sets default values
AGoobeyKillZVolume::AGoobeyKillZVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
}

// Called when the game starts or when spawned
void AGoobeyKillZVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGoobeyKillZVolume::OnBoxBeginOverlap);
}

void AGoobeyKillZVolume::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->ActorHasTag("Player"))
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

		if(FoundActors.Num() > 0)
		{
			OtherActor->SetActorLocation(FoundActors[0]->GetActorLocation());
		}
	}
}

// Called every frame
// void AGoobeyKillZVolume::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

