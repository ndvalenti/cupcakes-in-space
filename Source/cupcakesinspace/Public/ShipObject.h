// Fill out your copyright notice in the Description page of Project Settings.

/*
 * In order to add a custom class, add the proper parent and generated include files
 * and add UCLASS() and GENERATED_BODY(), after delete the 
 * .vs, Binaries, Intermediate, and Saved folders and *.sln file and
 * regenerate project files, ensure class prefixes are correct
 */

#pragma once

#include "CoreMinimal.h"

#include "Object.h"
#include "Int64Vector.h"
#include "StaticUtilities.h"
#include "EngineUtils.h"

#include "ShipObject.generated.h"


class AGameStateManager;

/*Tracking spawn and update fidelity for ships*/
UENUM(BlueprintType)
enum class EShipDistance : uint8
{
	SH_EMPTY	UMETA(DisplayName = "None"),
	SH_EXTERNAL	UMETA(DisplayName = "Different System"),
	SH_DOCKED	UMETA(DisplayName = "Docked In System"),
	SH_PLAYER	UMETA(DisplayName = "Player"),
	SH_FAR		UMETA(DisplayName = "Distant"),
	SH_NEAR		UMETA(DisplayName = "Near"),
	SH_LOCAL	UMETA(DisplayName = "Local")
};


UCLASS()
class CUPCAKESINSPACE_API UShipObject : public UObject
{
	GENERATED_BODY()

	friend AGameStateManager;
public:
	UShipObject();

protected:
	/**
	* Standard function to create a ship object -- currently does not support spawning in a docked manner
	*
	* @param Type - FString of the ship to be spawned
	* @param InitialPosition - Position in the system that the ship occupies
	* @param ShipIndex - The key reference for the asset map, this can change and as such should be used through safe interfaces
	*/
	void InitializeShip(
		const FString& Type,
		const FInt64Vector& InitialPosition,
		const int32& ShipIndex,
		const FName& System
	);

	/**
	* Recalculate and set the distance group this ship's distance group
	*
	* @param ShipDistance - The EShipDistance enum value the ship will be set to
	*/
	void SetDistanceGroup(const EShipDistance ShipDistance);

	/**
	* Mark this ship as the player ship
	*/
	void FlagAsPlayer();

	/**
	* Sets the imprecise location using the precise one
	*/
	void UpdateImpreciseLocation();

	/**
	* Sets this ships current system; Note this function does not check for parameter validity
	*
	* @param In - The system to move this ship to
	*/
	void SetSystem(FName& In);

public:

	/*True if this ship is docked*/
	UPROPERTY(VisibleAnywhere)
	bool bDocked;

	/*True if this ship is currently spawned*/
	UPROPERTY(VisibleAnywhere)
	bool bSpawned;

	/**
	* Get the distance group this ship belongs to
	*
	* @param Out - The EShipDistance enum value the ship currently belongs to
	*/
	void GetDistanceGroup(EShipDistance& Out) const;

	/**
	* Get the distance group this ship belongs to
	*
	* @return the EShipDistance enum value the ship currently belongs to
	*/
	EShipDistance GetDistanceGroup() const;

	/**
	* Gets the the vector and rotator needed to spawn this shipobject
	*
	* @param OutVector - The current location of this ship
	* @param OutRotation - This ships initial rotation when being spawned
	*/
	void GetSpawnPositionAndRotation(FVector& OutVector, FRotator& OutRotation) const;

	/**
	* Calculates and sets a new distance group for this ship
	*
	* @param GameStateManager - A pointer to the GameStateManager so ships can be initialized before GetWorld() is available
	* @param bPlayerExists - Optional parameter, should be false only if the player is not currently in a ship
	*/
	EShipDistance CalculateNewDistanceGroup(AGameStateManager* GameStateManager, bool bPlayerExists = true) const;

	/**
	* Get the current location of this ship in its current system
	*
	* @param Out - The current precise location of this ship
	*/
	void GetPreciseLocation(FInt64Vector& Out) const;

	/**
	* Get the current index of this ship
	*
	* @param Out - The current index of this ship
	*/
	void GetShipIndex(int32& Out) const;

	/**
	* Current system coordinates in light minutes
	*
	* param Out - The current imprecise location of this ship
	*/
	void GetImpreciseLocation(FVector& Out) const;

	/**
	* Get this ship's type for spawning
	*
	* @param Out - The name of the blueprint this ship associates with
	*/
	void GetShipType(FString& Out) const;

	/**
	* Gets this ship's current system
	*
	* @param Out - The name of the ship's current system
	*/
	void GetShipSystem(FName& Out) const;

	/**
	* Currently acting as a Debug Function to be replaced
	* Final implementation will return a formatted distance string in a less intensive manner
	* Get a formatted string indicating the distance from the current ship
	* to the chosen point
	*
	* @param From - The location to measure to
	* @return a formatted string indicating how far From is from the current ship
	*
	*/
	FString GetDisplayDistanceTo(const FInt64Vector& From) const;

	/**
	* Get a properly formatted string version of a float
	*
	* @param Number - The number to convert to a string
	* @param Precision - The precision of the resultant string
	* @return the provided number converted to a string
	*/
	FString GetFloatAsString(const float Number, const int32 Precision) const;

	/**
	*
	*/
	void TeleportToPreciseLocation(const FInt64Vector& Destination);

protected:
	/*String representation of this ship's asset name for spawning*/
	UPROPERTY(VisibleAnywhere)
	FString _ShipType;

	/*This ship's current system*/
	UPROPERTY(VisibleAnywhere)
	FName _System;

	/*Ship position in decimeters as FInt64Vector*/
	UPROPERTY(VisibleAnywhere)
	FInt64Vector _PreciseShipLocation;

	/*Ship position in light minutes, currently derived from PreciseShipLocation*/
	UPROPERTY(VisibleAnywhere)
	FVector _LongShipLocation;

	/*The distance group this ship is a member of*/
	UPROPERTY(VisibleAnywhere)
	EShipDistance _ShipDistanceGroup;

	/*Index key of this ship as it is stored in the Master Ship List*/
	int32 _ShipIndex;
};
