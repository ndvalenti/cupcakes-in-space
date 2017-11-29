// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "ExternalPlayerController.h"
#include "ExternalClickablePawn.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUPCAKESINSPACE_API UExternalClickablePawn : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UExternalClickablePawn();
	UFUNCTION(BlueprintCallable)
	void SetParentAsClicked();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	AExternalPlayerController *PlayerController;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
