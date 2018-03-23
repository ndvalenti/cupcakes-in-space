// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ScriptDelegates.h"

#include "Int64Vector.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Object.h"

#include "LocalPawn.generated.h"

class AGameStateManager;
class AMasterPlayerController;

UENUM(BlueprintType)
enum class EObjectClass : uint8
{
	OBCL_NONE	UMETA(DisplayName = "None"),
	OBCL_SHIP	UMETA(DisplayName = "Ship")
};

UCLASS()
class CUPCAKESINSPACE_API ALocalPawn : public APawn
{
	GENERATED_BODY()

public:
	ALocalPawn();
	UPROPERTY(EditAnywhere, AssetRegistrySearchable)
	EObjectClass ObjectClass;

protected:
	virtual void BeginPlay() override;

	/*Will rename upon deprication of PawnClicked*/
	UPROPERTY(EditAnywhere)
	bool bClickable;

	AGameStateManager* Manager;

public:	
	virtual void Tick(float DeltaTime) override;

	/*To be depricated, will be replaced by a raycast and this class will reparent to AActor*/
	UFUNCTION()
	void PawnClicked(AActor* TouchedActor, FKey ButtonPressed);
};
