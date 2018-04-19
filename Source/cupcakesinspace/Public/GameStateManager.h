// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "Int64Vector.h"
#include "AssetRegistryModule.h"
#include "ARFilter.h"
#include "ShipObject.h"
#include "ExternalCamera.h"

#include "Engine/StreamableManager.h"

#include "GameStateManager.generated.h"

class AExternalCamera;
class ALocalSpawnableObject;
class ACelestialController;

USTRUCT()
struct FSpawnPackage
{
	GENERATED_BODY()
	FSoftObjectPath AssetPath;
	FVector Location;
	FRotator Rotation;
	int32 Index;
};

UCLASS()
class CUPCAKESINSPACE_API AGameStateManager : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGameStateManager();

	/**Blueprint callable functions for setting up the level during loading*/
	/**
	*
	*/
	UFUNCTION(BlueprintCallable)
	void PopulateShipData();

	/**
	*
	*/
	UFUNCTION(BlueprintCallable)
	FName GetBridgeLevel() const;

	/**
	*
	*/
	UFUNCTION(BlueprintCallable)
	FName GetSystemLevel() const;

	/**
	*
	*/
	UFUNCTION(BlueprintCallable)
	FName GetLocalLevel() const;

	/**
	*
	*/
	void InitializeBackgroundLevelLoad();

	/**Functions to manipulate ships in the universe*/
	/**
	*
	*/
	bool PlayerShipExists() const;

	/**
	* Iterates through entire ship list and sorts each into their respective groups
	*/
	void SortAllShipDistanceGroups();

	/**
	* Moves Ship at ShipIndex from one distance group to another
	*
	* @param ShipIndex - The index of the ship to move
	* @param From - The group the ship is to be removed from
	* @param To - The group in which the ship will be placed
	*/
	void MoveShipToGroup(const int32& ShipIndex, const EShipDistance From, const EShipDistance To);

	/**
	* Create and initialize a new ship object
	*
	* @param ShipType - The blueprint class name of the ship to be created
	* @param ShipLocation - The initial coordinates of this ship
	* @param System - The system in which to place this ship
	* @return the index of the ship created
	*/
	int32 CreateNewShip(const FString& ShipType, const FInt64Vector& ShipLocation, const FName& System);

	/**
	* Set a ship as the new player ship
	*
	* @param ShipIndex - The index of the new player's ship
	*/

	/**
	*
	*/
	void SetAsPlayerShip(const int32& ShipIndex);

	/**
	*
	*/
	UShipObject* GetPlayerShipObject();

	void SetSpawnParameters();

	/**Functions to manipulate the local game space*/

	bool SpawnShip(UShipObject* Ship);

	void DespawnShip(ALocalSpawnableObject* Ship);

	void SpawnLocalShips();
	
	void SpawnPlayerShip();

	void LoadShipsInArea();

	void WarpShip(ALocalSpawnableObject* LocalSpawnedShip, const FInt64Vector& Destination);

	void WarpShip(const int32 Index, const FInt64Vector& Destination);

	void TeleportShip(const int32 ShipIndex, const FInt64Vector& Destination);

	/**
	* #Delegate Only: For Internal Use#
	* Spawns a ship in local space based on supplied spawnpackage after assets are confirmed loaded in memory
	*
	* @param SpawnPackage - The collection of information needed to spawn a ship
	*/
	void SpawnAssetInWorld(FSpawnPackage SpawnPackage, UShipObject* Ship);

	/**
	*
	*/
	void SetAndFocusGlobalOffset(const FInt64Vector& Offset);

	/**
	*
	*/
	void SetPossessedObject(AExternalCamera* In);

	/**Standard asset tracking containers*/
	/**Contains all the ship objects in the world*/
	UPROPERTY(VisibleAnywhere, Category = "Ships")
	TMap<int32, UShipObject*> ShipMasterList;

	/**Stores ships that are distant but in the same system*/
	UPROPERTY(VisibleAnywhere, Category = "Ships")
	TArray<int32> FarShips;

	/**Stores ships that are near to spawning range*/
	UPROPERTY(VisibleAnywhere, Category = "Ships")
	TArray<int32> NearShips;

	/**Stores ships that are close enough to be spawned*/
	UPROPERTY(VisibleAnywhere, Category = "Ships")
	TArray<int32> LocalShips;

	/**Stores ships that are in a different system than the player*/
	UPROPERTY(VisibleAnywhere, Category = "Ships")
	TArray<int32> OutSystemShips;

	/**Stores ships that are docked in the same system as the player*/
	UPROPERTY(VisibleAnywhere, Category = "Ships")
	TArray<int32> DockedShips;

	/***/
	UPROPERTY(VisibleAnywhere, Category = "Spawned Objects")
	TArray<ALocalSpawnableObject*> SpawnedShips;

	/***/
	UPROPERTY(VisibleAnywhere, Category = "Spawned Objects")
	ALocalSpawnableObject* SpawnedPlayerShip;

	/***/
	UPROPERTY(VisibleAnywhere, Category = "Celestials")
	TArray<ACelestialController*> Celestials;

	/**List of assets that can be loaded*/
	TArray<FSoftObjectPath> ShipLoadableAssetList;

	/**Container for spawning a ship asset*/
	FPrimaryAssetType ShipPrimaryAssetType;

protected:
	/*Streamable manager for recovering assets*/
	FStreamableManager Streamable;

	/**Contains the index of the player's currently boarded ship, -1 if it does not exist*/
	UPROPERTY(EditAnywhere, Category = "Ships")
	int32 PlayerShipIndex;

	/**A list of all ship assets in memory*/
	UPROPERTY(VisibleAnywhere, Category="Assets")
	TArray<FSoftObjectPath> ShipAssetList;

	/**The index of the next ship to be spawned*/
	UPROPERTY(VisibleAnywhere)
	int32 _ShipIndex;

	/**The current bridge level, dictated by the player's current ship*/
	UPROPERTY(EditAnywhere)
	FName _BridgeLevel;

	/**The system the player is currently in*/
	UPROPERTY(EditAnywhere)
	FName _SystemLevel;

	/**The name of the level that will display local objects*/
	UPROPERTY(EditAnywhere)
	FName _LocalLevel;

	/**Parameters for spawning all objects in local space*/
	FActorSpawnParameters ExternalSpawnParameters;

	/**
	* Recalculate and set the distance group of this ship
	*
	* @param Ship - The ship to update
	* @return the ship distance group the ship was moved to
	*/
	EShipDistance UpdateShipDistanceGroup(UShipObject* Ship);

	/**
	* Recalculate and set the distance group of this ship
	*
	* @param Ship - The ship to update
	* @param System - The system the ship is currently in
	* @return the ship distance group the ship was moved to
	*/
	EShipDistance UpdateShipDistanceGroup(UShipObject* Ship, const FName& System);

	/**
	* Recalculate the distance group of this ship
	*
	* @param Ship - The ship to update
	* @return the ship distance group the ship should be in
	*/
	EShipDistance CalculateTargetDistanceGroup(const UShipObject* Ship);

	/**
	* Recalculate the distance group of this ship
	*
	* @param Ship - The ship to update
	* @param System - The system the ship is currently in
	* @return the ship distance group the ship should be in
	*/
	EShipDistance CalculateTargetDistanceGroup(const UShipObject* Ship, const FName& System);

	/**
	* #For internal use only#
	* Adds a ship to the specified group
	*
	* @param ShipIndex - The index of the ship to be added
	* @param To - The group to add the ship to
	*/
	void AddShipToGroup(const int32& ShipIndex, const EShipDistance To);

	/**
	* #For internal use only#
	* Removes a ship to the specified group
	*
	* @param ShipIndex - The index of the ship to be removed
	* @param From - The group to remove the ship from
	*/
	void RemoveShipFromGroup(const int32& ShipIndex, const EShipDistance From);

	/**
	* #For internal use only#
	* Adds a ship to the local group
	*
	* @param ShipIndex - The index of the ship to be added
	*/
	void AddToLocal(const int32& ShipIndex);

	/**
	* #For internal use only#
	* Removes a ship from the local group
	*
	* @param ShipIndex - The index of the ship to be removed
	*/
	void RemoveFromLocal(const int32& ShipIndex);

	/**
	*
	*
	*
	*/
	void UpdateExternalLevel();
};
