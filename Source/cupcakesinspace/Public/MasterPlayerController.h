// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Int64Vector.h"
#include "GameStateManager.h"
#include "UObject/UObjectGlobals.h"
//#include "Engine/StreamableManager.h"
#include "EngineUtils.h"
#include "Containers/Queue.h"
#include "StaticUtilities.h"

#include "MasterPlayerController.generated.h"

//USTRUCT()
//struct FSpawnPackage
//{
//	GENERATED_BODY()
//	FSoftObjectPath AssetPath;
//	FVector Location;
//	FRotator Rotation;
//	int32 Index;
//};

UCLASS()
class CUPCAKESINSPACE_API AMasterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	/*Pointer to the game state manager*/
	AGameStateManager* GameStateManager;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitializeLevelLoad();
};
