// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "CelestialController.generated.h"

UCLASS()
class CUPCAKESINSPACE_API ACelestialController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACelestialController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector PlanetLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TLazyObjectPtr<AActor> ExternalCamera;
	
};
