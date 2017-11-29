// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanetController.h"


// Sets default values
APlanetController::APlanetController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlanetController::BeginPlay()
{
	Super::BeginPlay();
	SetTickGroup(TG_PostUpdateWork);
	PlanetLocation = GetActorLocation();
}

// Called every frame
void APlanetController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Skybox) {
		SetActorLocation(Skybox->GetActorLocation() + PlanetLocation);
	}
}

