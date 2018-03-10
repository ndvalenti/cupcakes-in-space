// Fill out your copyright notice in the Description page of Project Settings.

#include "CelestialController.h"


// Sets default values
ACelestialController::ACelestialController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACelestialController::BeginPlay()
{
	Super::BeginPlay();
	SetTickGroup(TG_PostUpdateWork);
	PlanetLocation = GetActorLocation();
	CurrCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

// Called every frame
void ACelestialController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrCamera) {
		SetActorLocation(CurrCamera->GetActorLocation() + PlanetLocation);
	}
}

