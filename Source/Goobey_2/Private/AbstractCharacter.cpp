//Property of The Goobey Gang LLC

#include "AbstractCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h" 
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h" 
#include "Components/AudioComponent.h" 
#include "Components/DecalComponent.h" 
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/GameplayStatics.h" 
#include "Math/UnrealMathUtility.h"
#include "Components/TimelineComponent.h" 
#include "GoobeyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values
AAbstractCharacter::AAbstractCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//set defaults
	bCanCycle = true;
	bIsOnWater = false;
	bCanMove = true;
	bCanFloatOnWater = true;
	bCanTurn = true;
	bIsTurning = false;
	bIsHoldingAction = false;
	bCanPerformAction = true;
	bCharacterCanCycle = true;
	bCanPause = true;
	bPlayerCanTurn = true;
	
	TurnTimerTime = 0.01f;
	TurnMultiplier = 2.0f;

	XMovement = 0.0f;
	YMovement = 0.0f;

	CurrDirection = EDirection::Walk_Forward;

	//creates the flipbook
	Flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook component"));
	Flipbook->SetupAttachment(GetMesh());
	Flipbook->SetRelativeRotation(FRotator(0, 90, 0));

	//spring arm for the camera
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	//the actual camera
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	//the bottom collision box
	BottomCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	BottomCollisionBox->InitBoxExtent(FVector(32, 32, 10));
	BottomCollisionBox->SetRelativeTransform(FTransform(FRotator(0, 0, 0), FVector(0, 0, -80)));
	BottomCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BottomCollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BottomCollisionBox->SetGenerateOverlapEvents(true);
	BottomCollisionBox->SetupAttachment(RootComponent);

	//action sound
	ActionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound Component"));
	ActionSound->bAutoActivate = false;
	ActionSound->SetupAttachment(RootComponent);

	//drop shadow
	CharacterShadow = CreateDefaultSubobject<UDecalComponent>(TEXT("Character Shadow"));
	CharacterShadow->DecalSize = FVector(250, 32, 32);
	CharacterShadow->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	CharacterShadow->SetRelativeLocation(FVector(0, 0, -350));
	CharacterShadow->SetupAttachment(RootComponent);

	WaterTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComp"));

	Tags.Add("Player");
}

// Called when the game starts or when spawned
void AAbstractCharacter::BeginPlay()
{
	Super::BeginPlay();

	TalkComp = AddComponentByClass(TalkComponent, true, FTransform(), false);

	BottomCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAbstractCharacter::OnBoxBeginOverlap);
	BottomCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AAbstractCharacter::OnBoxEndOverlap);

	if (vCurve)
	{
		FOnTimelineVector InterpFunction{};
		InterpFunction.BindUFunction(this, FName{ TEXT("OnTimelineTick") });
		WaterTimeline->AddInterpVector(vCurve, InterpFunction, FName("Curve"));
		WaterTimeline->SetLooping(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input a curve to play!"));
	}

	
	Cast<AGoobeyPlayerController>(GetWorld()->GetFirstPlayerController())->SetControlRotationAndBroadcast(GetActorRotation());


	ChildBeginPlay();
}

// Called every frame
void AAbstractCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bCanCycle = GetCanCycle();

	if (bIsOnWater && bCanFloatOnWater)
	{
		WaterTimeline->Play();
	}
	
	ChildTick();
}

// Called to bind functionality to input
void AAbstractCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAbstractCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAbstractCharacter::MoveRight);

	PlayerInputComponent->BindAction("LookLeft", IE_Pressed, this, &AAbstractCharacter::LookLeft);
	PlayerInputComponent->BindAction("LookRight", IE_Pressed, this, &AAbstractCharacter::LookRight);

	PlayerInputComponent->BindAction("CharacterAction", IE_Pressed, this, &AAbstractCharacter::PrepareCharacterAction);
	PlayerInputComponent->BindAction("CharacterAction", IE_Released, this, &AAbstractCharacter::PrepareReleaseCharacterAction);

	PlayerInputComponent->BindAction("OpenMenu", IE_Released, this, &AAbstractCharacter::OpenPauseMenu);
}

bool AAbstractCharacter::CanCycle() const
{
	return bCanCycle;
}

void AAbstractCharacter::MoveForward(float Value)
{
	if (bCanMove)
	{
		const FRotator Rot = FRotator(0, GetControlRotation().Yaw, 0);
		const FVector Vec = UKismetMathLibrary::GetForwardVector(Rot);
		AddMovementInput(Vec, Value);
		XMovement = Value;
		SetFlipbook();
	}
}

void AAbstractCharacter::MoveRight(float Value)
{
	if (bCanMove)
	{
		const FRotator Rot = FRotator(0, GetControlRotation().Yaw, 0);
		const FVector Vec = UKismetMathLibrary::GetRightVector(Rot);
		AddMovementInput(Vec, Value);
		YMovement = Value;
		SetFlipbook();
	}
}

bool AAbstractCharacter::SetDirectionAndFlipBook(EDirection direction)
{
	CurrDirection = direction;

	switch (direction)
	{
	case EDirection::Walk_Forward:
		return Flipbook->SetFlipbook(ForwardWalk);
		break;

	case EDirection::Walk_Backward:
		return Flipbook->SetFlipbook(BackWalk);
		break;

	case EDirection::Walk_Left:
		return Flipbook->SetFlipbook(LeftWalk);
		break;

	case EDirection::Walk_Right:
		return Flipbook->SetFlipbook(RightWalk);
		break;
	}

	return true;
}

void AAbstractCharacter::SetFlipbook()
{
	if(bIsHoldingAction)
	{
		return;
	}
	
	if(XMovement == 0 && YMovement == 0)
	{
		switch (CurrDirection)
		{

		case EDirection::Walk_Forward:
			Flipbook->SetFlipbook(ForwardIdle);
			break;

		case EDirection::Walk_Backward:
			Flipbook->SetFlipbook(LeftIdle);
			break;

		case EDirection::Walk_Left:
			Flipbook->SetFlipbook(LeftIdle);
			break;

		case EDirection::Walk_Right:
			Flipbook->SetFlipbook(ForwardIdle);
			break;

		}
	}
	//Walking Animations
	else
	{
		if (YMovement > 0)
		{
			SetDirectionAndFlipBook(EDirection::Walk_Right);
		}
		else if (YMovement < 0)
		{
			SetDirectionAndFlipBook(EDirection::Walk_Left);
		}
		else if (XMovement > 0)
		{
			SetDirectionAndFlipBook(EDirection::Walk_Forward);
		}
		else if (XMovement < 0)
		{
			SetDirectionAndFlipBook(EDirection::Walk_Backward);
		}
	}
}

void AAbstractCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor->ActorHasTag("Water"))
	{
		bIsOnWater = true;
	}
}

void AAbstractCharacter::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Water"))
	{
		ExitWater();
	}
}

bool AAbstractCharacter::GetCanCycle_Implementation()
{
	return (CanJump() || bIsOnWater) && !bIsTurning && bCharacterCanCycle;
}

void AAbstractCharacter::OnTimelineTick(FVector vec) const
{
	GetMesh()->AddLocalOffset(vec);
}

void AAbstractCharacter::ExitWater()
{
	bIsOnWater = false;

	if (bCanFloatOnWater)
	{
		GetMesh()->SetRelativeLocation(FVector(0, 0, 0));
		WaterTimeline->Stop();
	}
}

void AAbstractCharacter::LookLeft()
{
	if(bPlayerCanTurn)
		PrepareTurn(45.0f);
}

void AAbstractCharacter::LookRight()
{
	if(bPlayerCanTurn)
		PrepareTurn(-45.0f);
}

void AAbstractCharacter::PrepareTurn(float target)
{
	if (bCanTurn)
	{
		bCanTurn = false;
		bIsTurning = true;
		FTimerDelegate TimerDel;
		TimerDel.BindUFunction(this, FName("TurnTick"), target, UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation().Yaw + target);
		GetWorld()->GetTimerManager().SetTimer(TurnTimerHandle, TimerDel, TurnTimerTime, true);
	}
}

void AAbstractCharacter::TurnTick(float Amount, float FinalAmount)
{
	AGoobeyPlayerController* PlayerController = Cast<AGoobeyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	const FRotator CRotator = PlayerController->GetControlRotation();

	const FRotator NewRot = FRotator(CRotator.Pitch, CRotator.Yaw + (Amount * TurnTimerTime * TurnMultiplier), CRotator.Roll);

	PlayerController->SetControlRotationAndBroadcast(NewRot);

	if (FMath::IsNearlyEqual(PlayerController->GetControlRotation().Yaw, FinalAmount, 0.01f))
	{
		GetWorld()->GetTimerManager().ClearTimer(TurnTimerHandle);
		bCanTurn = true;
		bIsTurning = false;
	}
}

bool AAbstractCharacter::CanMove()
{
	return bCanMove;
}

bool AAbstractCharacter::SetCanMove(bool newVal)
{
	return bCanMove = newVal;
}

bool AAbstractCharacter::IsHoldingAction()
{
	return bIsHoldingAction;
}

void AAbstractCharacter::PrepareCharacterAction()
{
	if (bCanPerformAction)
	{
		bIsHoldingAction = true;
		CharacterAction();
	}
}

void AAbstractCharacter::PrepareReleaseCharacterAction()
{
	if (bCanPerformAction)
	{
		bIsHoldingAction = false;
		ReleaseCharacterAction();
	}
}

bool AAbstractCharacter::CanFloatOnWater()
{
	return bCanFloatOnWater;
}

bool AAbstractCharacter::SetCanFloatOnWater(bool NewVal)
{
	return bCanFloatOnWater = NewVal;
}

bool AAbstractCharacter::GetCanPerformAction()
{
	return bCanPerformAction;
}

bool AAbstractCharacter::SetCanPerformAction(bool NewValue)
{
	return bCanPerformAction = NewValue;
}

void AAbstractCharacter::DisableMovementAndAction()
{
	bCanPerformAction = false;
	bCanMove = false;
}

void AAbstractCharacter::EnableMovementAndAction()
{
	bCanPerformAction = true;
	bCanMove = true;
}

bool AAbstractCharacter::GetCharacterCanCycle()
{
	return bCharacterCanCycle;
}

bool AAbstractCharacter::SetCharacterCanCycle(bool NewVal)
{
	return bCharacterCanCycle = NewVal;
}

float AAbstractCharacter::GetYMovement() const
{
	return YMovement;
}

float AAbstractCharacter::GetXMovement() const
{
	return XMovement;
}

void AAbstractCharacter::CharacterAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Not Implemented"));
}

void AAbstractCharacter::ReleaseCharacterAction_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Not Implemented"));
}

UActorComponent* AAbstractCharacter::GetTalkComp()
{
	return TalkComp;
}

void AAbstractCharacter::OpenPauseMenu()
{
	 UE_LOG(LogTemp, Warning, TEXT("Menu"));

	if(PauseMenuWidget && bCanPause)
	{
		MenuWidget = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), PauseMenuWidget);
		MenuWidget->AddToViewport();

		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	}
}

bool AAbstractCharacter::GetCanPause()
{
	return bCanPause;
}

bool AAbstractCharacter::SetCanPause(bool NewValue)
{
	return bCanPause = NewValue;
}

bool AAbstractCharacter::GetPlayerCanTurn()
{
	return bPlayerCanTurn;
}

bool AAbstractCharacter::SetPlayerCanTurn(bool NewValue)
{
	return bPlayerCanTurn = NewValue;
}