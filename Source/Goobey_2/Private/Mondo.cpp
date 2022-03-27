// Property of The Goobey Gang LLC


#include "Mondo.h"
#include "Kismet/KismetMathLibrary.h"

AMondo::AMondo()
{
	bCanPush = false;
	Tags.Add("Mondo");
}

void AMondo::MoveForward(const float Value)
{
	Super::MoveForward(Value);

	const FRotator Rot = FRotator(0, GetControlRotation().Yaw, 0);
	const FVector Vec = UKismetMathLibrary::GetForwardVector(Rot);
	BroadCastDirection(Vec, Value);
}

void AMondo::MoveRight(const float Value)
{
	Super::MoveRight(Value);

	const FRotator Rot = FRotator(0, GetControlRotation().Yaw, 0);
	const FVector Vec = UKismetMathLibrary::GetRightVector(Rot);
	BroadCastDirection(Vec, Value);
}

void AMondo::BroadCastDirection(const FVector Direction, float Value) const
{
	if(bCanMove && bCanPush)
	{		
		OnPushBlock.Broadcast(Direction, Value);
		// UE_LOG(LogTemp, Warning, TEXT("Mondo Push"));
	}
}

void AMondo::CharacterAction_Implementation()
{
	bCanPush = true;
	MondoAction();
}

void AMondo::ReleaseCharacterAction_Implementation()
{
	bCanPush = false;
}

void AMondo::BeginPlay()
{
	Super::BeginPlay();
}

void AMondo::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}