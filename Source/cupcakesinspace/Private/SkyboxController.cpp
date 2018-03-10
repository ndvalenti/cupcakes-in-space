// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyboxController.h"


// Sets default values
ASkyboxController::ASkyboxController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASkyboxController::BeginPlay()
{
	Super::BeginPlay();
	SetTickGroup(TG_PostUpdateWork);
	CurrCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

// Called every frame
void ASkyboxController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrCamera)	{
		SetActorLocation(CurrCamera->GetActorLocation());
	}
}

