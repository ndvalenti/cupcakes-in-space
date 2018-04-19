// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "AssetBundleData.h"
#include "SubclassOf.h"
#include "Engine/AssetManager.h"
#include "Int64Vector.h"
#include "GameStateManager.h"

#include "LocalSpawnableObject.generated.h"


UENUM(BlueprintType)
enum class EObjectClass : uint8
{
	OBCL_NONE	UMETA(DisplayName = "None"),
	OBCL_SHIP	UMETA(DisplayName = "Ship")
};


UCLASS(Blueprintable)
class CUPCAKESINSPACE_API ALocalSpawnableObject : public APawn
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	/*
	* Construct and return the PrimaryAssetId for use with the Asset Registry
	*
	* @return a PrimaryAssetId marking this object as a Ship
	*
	* In the future this should dynamically decide how to register this asset,
	* for now only ships exist
	*/
	FPrimaryAssetId GetPrimaryAssetId() const override;

	/*
	* Refreshes asset manager on Load, required for this to be a Primary Asset
	*/
	void PostLoad();

protected:
	/*Index of the ship object this represents in the Master List*/
	UPROPERTY(VisibleAnywhere)
	int32 _ShipIndex;

public:	
	ALocalSpawnableObject();

	bool DespawnShip();

	void WarpTo(const FInt64Vector& Destination);

	/*
	* Gets the ship index
	*
	* @return the variable that will be changed to match this object's index
	*/
	int32 GetShipIndex() const;

	/*
	* Set the ship index
	*
	* @param ShipIndex - The value this object's index will be changed to
	*/
	void SetShipIndex(int32 IndexIn);


private:
	UPROPERTY(AssetRegistrySearchable)
	TSubclassOf<ALocalSpawnableObject> NativeClass;

	UPROPERTY(VisibleAnywhere, AssetRegistrySearchable)
	EObjectClass ObjectClass;
};
