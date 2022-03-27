// Property of The Goobey Gang LLC


#include "TurnHandlerComponent.h"
#include "GoobeyPlayerController.h"
#include "Kismet/GameplayStatics.h" 

// Sets default values for this component's properties
UTurnHandlerComponent::UTurnHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

}


// Called when the game starts
void UTurnHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	AGoobeyPlayerController* PlayerController = Cast<AGoobeyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	PlayerController->OnControlTurn.AddDynamic(this, &UTurnHandlerComponent::AddRotation);

	GetOwner()->SetActorRotation(GetWorld()->GetFirstPlayerController()->GetControlRotation());
}

void UTurnHandlerComponent::AddRotation(FRotator NewRotation)
{
	GetOwner()->SetActorRotation(NewRotation);
}
