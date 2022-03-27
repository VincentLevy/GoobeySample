// Property of The Goobey Gang LLC


#include "MondoPushBlock.h"
#include "Mondo.h"
#include "Components/BoxComponent.h"

// Sets default values
AMondoPushBlock::AMondoPushBlock()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Comp"));
	CubeMesh->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Comp"));
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetupAttachment(CubeMesh);
}

// Called when the game starts or when spawned
void AMondoPushBlock::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AMondoPushBlock::OnBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AMondoPushBlock::OnEndOverlap);
}

void AMondoPushBlock::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->ActorHasTag("Mondo"))
	{
		Cast<AMondo>(OtherActor)->OnPushBlock.AddDynamic(this, &AMondoPushBlock::OnMondoPushBlock);
	}
}

void AMondoPushBlock::OnMondoPushBlock(const FVector Direction, const float Value)
{
	AddMovementInput(Direction, Value);
	UE_LOG(LogTemp, Warning, TEXT("Mondo is Pushing"));
}

//serves to unbind from the event
void AMondoPushBlock::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && OtherActor->ActorHasTag("Mondo"))
	{
		Cast<AMondo>(OtherActor)->OnPushBlock.RemoveAll(this);
	}
}
