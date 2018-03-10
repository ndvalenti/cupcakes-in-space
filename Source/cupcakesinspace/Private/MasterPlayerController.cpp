// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterPlayerController.h"


void AMasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
}

void AMasterPlayerController::Tick(float DeltaTime)
{

}

void AMasterPlayerController::SetCameraFocus(AActor *FocusedPawn)
{
	CameraFocus = FocusedPawn;
	UE_LOG(LogTemp, Warning, TEXT("Camera Possessed: %s"), *CameraFocus->GetName());
}
