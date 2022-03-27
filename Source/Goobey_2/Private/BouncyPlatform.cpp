// Property of The Goobey Gang LLC


#include "BouncyPlatform.h"
#include "Components/BoxComponent.h" 
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h" 


// Sets default values
ABouncyPlatform::ABouncyPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = CollisionBox;

	BounceSound = CreateDefaultSubobject<UAudioComponent>("BounceSound");
	BounceSound->SetupAttachment(CollisionBox);
	BounceSound->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<USoundWave> sound(TEXT("/Game/_Goobey2/Unused/GooeyThings/Sounds/jump_11.jump_11"));
	if (sound.Succeeded())
	{
		BounceSound->SetSound(sound.Object);
	}

	LaunchForce = 3000.0f;
}

// Called when the game starts or when spawned
void ABouncyPlatform::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABouncyPlatform::OnBoxBeginOverlap);
}

// Called every frame
//void ABouncyPlatform::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ABouncyPlatform::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{	
		Cast<ACharacter>(OtherActor)->LaunchCharacter(CollisionBox->GetUpVector() * FVector(LaunchForce, LaunchForce, LaunchForce), true, true);
		BounceSound->Play();
	}
}