// Fill out your copyright notice in the Description page of Project Settings.

#include "ExternalPlayerController.h"


void AExternalPlayerController::BeginPlay() 
{
	Super::BeginPlay();
	bEnableClickEvents = true;
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
}

void AExternalPlayerController::Tick(float DeltaTime)
{

}
/*
AExternalCamera* AExternalPlayerController::GetExternalCamera() const
{
	return Cast<AExternalCamera>(GetPawn());
}
*/

void AExternalPlayerController::SetCameraFocus(AActor *FocusedPawn)
{
	CameraFocus = FocusedPawn;
	UE_LOG(LogTemp, Warning, TEXT("Camera Possessed: %s"), *CameraFocus->GetName());
}
