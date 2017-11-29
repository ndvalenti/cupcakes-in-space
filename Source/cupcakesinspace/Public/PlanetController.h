// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "PlanetController.generated.h"

UCLASS()
class CUPCAKESINSPACE_API APlanetController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlanetController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector PlanetLocation;
	UPROPERTY(EditAnywhere)
	TLazyObjectPtr<AActor> Skybox;
	
};
