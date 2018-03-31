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

#include "ShipObject.generated.h"

UENUM(BlueprintType)
enum class EShipDistance : uint8
{
	SH_EMPTY	UMETA(DisplayName = "None"),
	SH_PLAYER	UMETA(DisplayName = "Player"),
	SH_FAR		UMETA(DisplayName = "Distant"),
	SH_NEAR		UMETA(DisplayName = "Near"),
	SH_LOCAL	UMETA(DisplayName = "Local")
};

UCLASS()
class CUPCAKESINSPACE_API UShipObject : public UObject
{
	GENERATED_BODY()

public:
	UShipObject();

	UPROPERTY(VisibleAnywhere)
	FString ShipType;

	UPROPERTY(VisibleAnywhere)
	EShipDistance ShipDistanceGroup;
	
	/*Must be called any time a new ship is created*/
	void InitializeShip(const FString& Type, const FInt64Vector& InitialPosition);
	
	/*Fills Vectors for distance and rotation of this ship for use in actor spawning*/
	void GetSpawnPositionAndRotation(FVector& OutVector, FRotator& OutRotation) const;
	
	void UpdateFarLocation();

	/*Recalculates this ship's distance group and returns it*/
	EShipDistance SetDistanceGroup();

	FVector ConvertCloseToFarLocation(const FInt64Vector& InVector);

	/*Current system coordinates in decimeters*/
	FInt64Vector GetCloseLocation();

	/*Current system coordinates in light minutes*/
	FVector GetFarLocation();

	/* 
	 * Currently acting as a Debug Function to be replaced
	 * Final implementation will return a formatted distance string in a less intensive manner
	 */
	FString GetDisplayDistanceTo(const FInt64Vector& From);
	
protected:
	/*Ship position in dm as int64vector*/
	UPROPERTY(VisibleAnywhere)
	FInt64Vector PreciseShipLocation;

	/*Ship position in light minutes, currently derived from PreciseShipLocation*/
	UPROPERTY(VisibleAnywhere)
	FVector LongShipLocation;

	/*Debug function for now, will be rewritten*/
	FString GetFloatAsString(float Number, int Precision);

	/*
	 * Index of this ship stored in the TMap
	 * Future functions will consolidate these but outside of this change only carefully
	 */
	int32 ShipIndex;
};
