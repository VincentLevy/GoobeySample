// Property of The Goobey Gang LLC


#include "BaseGameMode.h"
#include "Kismet/GameplayStatics.h" 
#include "GameFramework/PlayerStart.h" 
#include "AbstractCharacter.h"

ABaseGameMode::ABaseGameMode()
{
    CurrIndex = 0;
    CollectableCount = 0;
}

void ABaseGameMode::BeginPlay()
{
    PrimaryActorTick.bCanEverTick = false;
    PrimaryActorTick.bStartWithTickEnabled = false;

    //binds the cycle up/down events to the player controller
    //from https://answers.unrealengine.com/questions/191818/view.html
    UWorld* World = GetWorld();
    if (World)
    {
        //binds the inputs for cycling up and down
        World->GetFirstPlayerController()->InputComponent->BindAction("CycleUp", IE_Pressed, this, &ABaseGameMode::CycleUp);
        World->GetFirstPlayerController()->InputComponent->BindAction("CycleDown", IE_Pressed, this, &ABaseGameMode::CycleDown);
        
        World->GetFirstPlayerController()->InputComponent->BindAction("SpawnCharacter1", IE_Pressed, this, &ABaseGameMode::SpawnCharacter1);
        World->GetFirstPlayerController()->InputComponent->BindAction("SpawnCharacter2", IE_Pressed, this, &ABaseGameMode::SpawnCharacter2);
        World->GetFirstPlayerController()->InputComponent->BindAction("SpawnCharacter3", IE_Pressed, this, &ABaseGameMode::SpawnCharacter3);
        World->GetFirstPlayerController()->InputComponent->BindAction("SpawnCharacter4", IE_Pressed, this, &ABaseGameMode::SpawnCharacter4);
        World->GetFirstPlayerController()->InputComponent->BindAction("SpawnCharacter5", IE_Pressed, this, &ABaseGameMode::SpawnCharacter5);

    }

        //finds the player start
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

        //spawns the first character in the location of the first spawn
        if (FoundActors.Num() > 0 && Characters.Num() > 0 && Characters[0])
        {
            SpawnAndPosses(Characters[0], &FoundActors[0]->GetTransform());
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No player start or enough characters on the game mode!"));
            UE_LOG(LogTemp, Error, TEXT("No player start or enough characters on the game mode!"));
        }

    CollectableCount = 100;
}

void ABaseGameMode::CycleUp()
{
    if (CurrChar->CanCycle() && Characters.Num() > 1)
    {
        FTransform tf = CurrChar->GetActorTransform();
        CurrChar->Destroy();

        CurrIndex++;
        CheckCurrIndex();

        SpawnAndPosses(Characters[CurrIndex], &tf);
    }

}

void ABaseGameMode::CycleDown()
{
    if (CurrChar->CanCycle() && Characters.Num() > 1)
    {
        FTransform tf = CurrChar->GetActorTransform();
        CurrChar->Destroy();

        CurrIndex--;
        CheckCurrIndex();

        SpawnAndPosses(Characters[CurrIndex], &tf);
    }
}

void ABaseGameMode::SpawnAndPosses(UClass* Class, const FTransform* Transform)
{
    if(!CollisionCheckerTemplate)
    {
        UE_LOG(LogTemp, Error, TEXT("no variable set in for collision checker"));
        return;
    }
    
    UClass* PreviousChar = nullptr;
    //Destroys the previous current character
    if (CurrChar)
    {
        PreviousChar = CurrChar->GetClass();
        CurrChar->Destroy();
    }

    //the next character will not spawn if colliding
    FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
    AActor* ColCheck = GetWorld()->SpawnActor(CollisionCheckerTemplate, Transform, SpawnParameters);

    if(ColCheck)
    {
        //Only spawn the new Character if the current place is good to spawn a new Character
        ColCheck->Destroy();

        FActorSpawnParameters NewSpawnParameters = FActorSpawnParameters();
        NewSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        
        CurrChar = Cast<AAbstractCharacter>(GetWorld()->SpawnActor(Class, Transform, NewSpawnParameters));
    }
    else
    {
        //spawns the previous character
        FActorSpawnParameters NewSpawnParameters = FActorSpawnParameters();
        NewSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        CurrChar = Cast<AAbstractCharacter>(GetWorld()->SpawnActor(PreviousChar, Transform, NewSpawnParameters));
    }

    //Posses the new character
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    PlayerController->UnPossess();

    PlayerController->Possess(Cast<ACharacter>(CurrChar));
}

void ABaseGameMode::CheckCurrIndex()
{
    if (CurrIndex > Characters.Num() - 1)
        CurrIndex = 0;
    else if (CurrIndex < 0)
        CurrIndex = Characters.Num() - 1;
}

void ABaseGameMode::AddCharacterToParty(TSubclassOf<class AAbstractCharacter> NewCharacter, bool ShouldSpawnAndPosses)
{
    Characters.Emplace(NewCharacter);

    //Spawns the character and posses it in case ShouldSpawn and posses is set to true
    if (ShouldSpawnAndPosses)
    {
        SpawnAndPosses(NewCharacter, &CurrChar->GetActorTransform());
    }
}

void ABaseGameMode::RemoveCharacterFromParty(TSubclassOf<class AAbstractCharacter> RemoveCharacter)
{
    if (Characters.Num() < 2)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot remove character when there is only one!"));

        return;
    }

    if (Characters.Contains(RemoveCharacter))
    {
        Characters.Remove(RemoveCharacter);
        
        if (CurrChar->GetClass() == RemoveCharacter.Get())
        {
            SpawnAndPosses(Characters[0], &CurrChar->GetActorTransform());
        }
    }
}

void ABaseGameMode::IncrementCollectableCount()
{
    CollectableCount++;
}

int ABaseGameMode::GetCollectableCount()
{
    return CollectableCount;
}

void ABaseGameMode::SpawnCharacter1()
{
    PrepareSpawn(Character1);
}

void ABaseGameMode::SpawnCharacter2()
{
    PrepareSpawn(Character2);
}

void ABaseGameMode::SpawnCharacter3()
{
    PrepareSpawn(Character3);
}

void ABaseGameMode::SpawnCharacter4()
{
    PrepareSpawn(Character4);
}

void ABaseGameMode::SpawnCharacter5()
{
    PrepareSpawn(Character5);
}

void ABaseGameMode::PrepareSpawn(TSubclassOf<AAbstractCharacter> Character)
{
    if(CurrChar && Character && Characters.Contains(Character) && CurrChar->GetClass() != Character && CurrChar->CanCycle())
    {
        SpawnAndPosses(Character, &CurrChar->GetTransform());
    }
}

//returns a copy of the character array
//to add or remove characters use AddCharacterToParty or RemoveCharacterFromParty
TArray<TSubclassOf<class AAbstractCharacter>> ABaseGameMode::GetCharacters()
{
    TArray<TSubclassOf<class AAbstractCharacter>> ret (Characters);
    return ret;
}
