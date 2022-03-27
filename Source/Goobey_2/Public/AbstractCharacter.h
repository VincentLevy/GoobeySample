//Property of The Goobey Gang LLC

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbstractCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UPaperFlipbook;
class UPaperFlipbookComponent;
class UBoxComponent;
class UAudioComponent;
class UDecalComponent;
class UTimelineComponent;
class UCurveVector;

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Walk_Forward UMETA(DisplayName = "Forward"),
	Walk_Backward UMETA(DisplayName = "Backward"),
	Walk_Left UMETA(DisplayName = "Left"),
	Walk_Right UMETA(DisplayName = "Right"),
};

UCLASS()
class GOOBEY_2_API AAbstractCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAbstractCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//properties
	bool bCanCycle;
	bool bIsOnWater;
	bool bCanTurn;
	bool bIsTurning;
	bool bCanMove;
	bool bIsHoldingAction;
	bool bCanPerformAction;
	bool bCharacterCanCycle;
	bool bCanPause;
	bool bPlayerCanTurn;

	float TurnTimerTime;

	float XMovement;
	float YMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TurnMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	bool bCanFloatOnWater;

	EDirection CurrDirection;

	UUserWidget* MenuWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Flipbooks")
	UPaperFlipbookComponent* Flipbook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
	UPaperFlipbook* LeftWalk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
	UPaperFlipbook* BackWalk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
	UPaperFlipbook* ForwardWalk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
	UPaperFlipbook* RightWalk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
	UPaperFlipbook* ForwardIdle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flipbooks")
	UPaperFlipbook* LeftIdle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraComp")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraComp")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UBoxComponent* BottomCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UAudioComponent* ActionSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UDecalComponent* CharacterShadow;

	UPROPERTY()
	UTimelineComponent* WaterTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	UCurveVector* vCurve;

	UPROPERTY(EditAnywhere, Category = Players, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UActorComponent> TalkComponent;

	UActorComponent* TalkComp;
	
	FTimerHandle TurnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User Interface")
	TSubclassOf<UUserWidget> PauseMenuWidget;
	
	//member functions

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character")
	UActorComponent* GetTalkComp();
	
	UFUNCTION()
	void OnTimelineTick(FVector vec) const;

	UFUNCTION()
	void TurnTick(float Amount, float FinalAmount);

	void PrepareCharacterAction();
	void PrepareReleaseCharacterAction();

	UFUNCTION(BlueprintNativeEvent , Category = "Character")
	void CharacterAction();

	UFUNCTION(BlueprintNativeEvent, Category = "Character")
	void ReleaseCharacterAction();

	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	
	void OpenPauseMenu();

	void LookLeft();
	void LookRight();
	void PrepareTurn(float target);

	bool SetDirectionAndFlipBook(EDirection direction);
	void SetFlipbook();

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	bool GetCanCycle();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
	void ChildBeginPlay();

	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
	void ChildTick();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	bool IsHoldingAction();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	bool CanMove();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool SetCanMove(bool newVal);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ExitWater();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Movement")
	bool CanFloatOnWater();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool SetCanFloatOnWater(bool NewVal);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character Action")
	bool GetCanPerformAction();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	bool SetCanPerformAction(bool NewValue);

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	void DisableMovementAndAction();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	void EnableMovementAndAction();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Character Action")
	bool GetCharacterCanCycle();

	UFUNCTION(BlueprintCallable, Category = "Character Action")
	bool SetCharacterCanCycle(bool NewVal);

	UFUNCTION(BlueprintPure, Category = Movement)
	float GetYMovement() const;

	UFUNCTION(BlueprintPure, Category = Movement)
	float GetXMovement() const;

	UFUNCTION(BlueprintPure, Category = Dialogue)
	bool GetCanPause();

	UFUNCTION(BlueprintCallable, Category = Dialogue)
	bool SetCanPause(bool NewValue);

	UFUNCTION(BlueprintPure, Category = Dialogue)
	bool GetPlayerCanTurn();

	UFUNCTION(BlueprintCallable, Category = Dialogue)
	bool SetPlayerCanTurn(bool NewValue);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	bool CanCycle() const;
};
