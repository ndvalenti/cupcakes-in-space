// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EngineUtils.h"
#include "Int64Vector.h"
#include "StaticUtilities.h"
#include "Kismet/GameplayStatics.h"

#include "CelestialController.generated.h"

//class AGameStateManager;

UCLASS()
class CUPCAKESINSPACE_API ACelestialController : public AActor
{
	GENERATED_BODY()
	
public:	
	ACelestialController();

protected:
	virtual void BeginPlay() override;

public:	
	/*Contains the location of the celestial scaled to the global offset*/
	UPROPERTY(EditAnywhere, Category = Positional)
	FInt64Vector TrueLocation;

	UPROPERTY(VisibleAnywhere, Category = Informational)
	FVector AbstractLocation;

	UPROPERTY(VisibleAnywhere, Category = Informational)
	TLazyObjectPtr<AActor> CurrentCamera;

	UPROPERTY(VisibleAnywhere, Category = Informational)
	double ScaleRatio;

	UPROPERTY(EditAnywhere, Category = Positional)
	int64 Size;

	virtual void Tick(float DeltaTime) override;

	void UpdateScaleRatio();
};
