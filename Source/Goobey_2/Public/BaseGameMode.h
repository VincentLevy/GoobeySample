// Property of The Goobey Gang LLC

#pragma once

#include "CoreMinimal.h"

#include "CollisionChecker.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMondoSpawnDelegate, bool, Spawned);

class AAbstractCharacter;

/**
 * 
 */
UCLASS()
class GOOBEY_2_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	void CycleUp();
	void CycleDown();
	void SpawnAndPosses(UClass* Class, const FTransform* Transform);
	void CheckCurrIndex();
	
	void SpawnCharacter1();
	void SpawnCharacter2();
	void SpawnCharacter3();
	void SpawnCharacter4();
	void SpawnCharacter5();

	void PrepareSpawn(TSubclassOf<class AAbstractCharacter> Character);

	UFUNCTION(BlueprintPure, Category = "Characters")
	TArray<TSubclassOf<class AAbstractCharacter>> GetCharacters();

	UPROPERTY(EditAnywhere, Category = Characters, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACollisionChecker> CollisionCheckerTemplate;
	
	//properties
	int32 CurrIndex;
	AAbstractCharacter* CurrChar;

	int CollectableCount;

	UPROPERTY(EditAnywhere, Category = Characters, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class AAbstractCharacter>> Characters;

	UPROPERTY(EditAnywhere, Category = Characters, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAbstractCharacter> Character1;

	UPROPERTY(EditAnywhere, Category = Characters, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAbstractCharacter> Character2;

	UPROPERTY(EditAnywhere, Category = Characters, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAbstractCharacter> Character3;

	UPROPERTY(EditAnywhere, Category = Characters, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAbstractCharacter> Character4;

	UPROPERTY(EditAnywhere, Category = Characters, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAbstractCharacter> Character5;
	
	UFUNCTION(BlueprintCallable, Category = "Characters")
	void AddCharacterToParty(TSubclassOf<class AAbstractCharacter> NewCharacter, bool ShouldSpawnAndPosses);

	UFUNCTION(BlueprintCallable, Category = "Characters")
	void RemoveCharacterFromParty(TSubclassOf<class AAbstractCharacter> NewCharacter);

	UFUNCTION(BlueprintCallable, Category = "Collectables")
	void IncrementCollectableCount();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Collectables")
	int GetCollectableCount();
	
public:
	//set the defaults
	ABaseGameMode();
	
	FMondoSpawnDelegate OnMondoSpawn;
};
