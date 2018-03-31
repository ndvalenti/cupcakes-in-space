// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "Int64Vector.h"
#include "AssetRegistryModule.h"
#include "ARFilter.h"
#include "ShipObject.h"

#include "GameStateManager.generated.h"


UCLASS()
class CUPCAKESINSPACE_API AGameStateManager : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AGameStateManager();
	UPROPERTY(VisibleAnywhere, Category="Asset Master Lists")
	TArray<FSoftObjectPath> ShipAssetList;

	UPROPERTY(VisibleAnywhere, Category = "Ship Lists")
	TMap<int32, UShipObject*> ShipMasterList;

	/*Order unimportant, used to track which ships are distant in same system*/
	UPROPERTY(VisibleAnywhere, Category = "Ship Lists")
	TArray<int> FarShips;

	/*Order unimportant, used to track which ships are near in same system*/
	UPROPERTY(VisibleAnywhere, Category = "Ship Lists")
	TArray<int> NearShips;

	/*Order unimportant, used to track which ships are close enough to be spawned*/
	UPROPERTY(VisibleAnywhere, Category = "Ship Lists")
	TArray<int> LocalShips;

	UPROPERTY(EditAnywhere, Category="Ship Lists")
	UShipObject* PlayerShip;

	UFUNCTION(BlueprintCallable)
	FName GetBridgeLevel();
	UFUNCTION(BlueprintCallable)
	FName GetSystemLevel();

protected:
	UFUNCTION()
	void OnFilesLoaded();
	UFUNCTION()
	void BuildSampleList();
	UPROPERTY(VisibleAnywhere)
	int32 ShipIndex;
	UPROPERTY(EditAnywhere)
	FName BridgeLevel;
	UPROPERTY(EditAnywhere)
	FName SystemLevel;
};
