// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ScriptDelegates.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MasterPlayerController.h"

#include "LocalPawn.generated.h"

class AGameStateManager;

UCLASS()
class CUPCAKESINSPACE_API ALocalPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALocalPawn();
	UPROPERTY(EditAnywhere)
	bool bClickable;

protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	AGameStateManager* Manager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void PawnClicked(AActor* TouchedActor, FKey ButtonPressed);
	//AActor* PawnClicked(AActor* TouchedActor, FKey ButtonPressed);
};
