// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Kismet/GameplayStatics.h"

#include "SkyboxController.generated.h"

UCLASS()
class CUPCAKESINSPACE_API ASkyboxController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkyboxController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector PreviousDesiredLoc;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	UPROPERTY(EditAnywhere)
	TLazyObjectPtr<AActor> CurrCamera;

	
	
};
