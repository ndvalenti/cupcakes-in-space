// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Int64Vector.h"
#include "GameStateManager.h"
#include "UObject/UObjectGlobals.h"
#include "Engine/StreamableManager.h"
#include "EngineUtils.h"
#include "Containers/Queue.h"
#include "StaticUtilities.h"

#include "MasterPlayerController.generated.h"

USTRUCT()
struct FSpawnPackage
{
	GENERATED_BODY()
	FSoftObjectPath AssetPath;
	FVector Location;
	FRotator Rotation;
};

UCLASS()
class CUPCAKESINSPACE_API AMasterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*Array of levels that are currently loaded, implementation currently very limited*/
	UPROPERTY(EditAnywhere, Category = Debug)
	TArray<ULevel*> LoadedLevels;

	/* 
	 * Spawns all ships at current origin, for use in initial area loading
	 * This needs additional functions to preload and unload which are unimplemented
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnAllAtLocation();

	/*Spawns the supplied ship object in the world*/
	UFUNCTION()
	bool SpawnShip(UShipObject* Ship);

	AGameStateManager* GameStateManager;
	FStreamableManager Streamable;

protected:
	UFUNCTION()
	void SpawnAssetInWorld(FSpawnPackage SpawnPackage);

	TArray<FSoftObjectPath> ShipLoadableAssetList;
	FPrimaryAssetType ShipPrimaryAssetType;
};
