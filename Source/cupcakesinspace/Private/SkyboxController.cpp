// Fill out your copyright notice in the Description page of Project Settings.

#include "SkyboxController.h"


ASkyboxController::ASkyboxController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASkyboxController::BeginPlay()
{
	Super::BeginPlay();
	SetTickGroup(TG_PostUpdateWork);
	CurrentCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

void ASkyboxController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentCamera)
	{
		SetActorLocation(CurrentCamera->GetActorLocation());
	}
}
