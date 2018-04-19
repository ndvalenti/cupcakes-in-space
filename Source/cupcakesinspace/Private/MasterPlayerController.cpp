// Fill out your copyright notice in the Description page of Project Settings.
#include "MasterPlayerController.h"
#include "Engine/AssetManager.h"
#include "CelestialController.h"
#include "LocalSpawnableObject.h"


void AMasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bEnableClickEvents = true;
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;

	AGameStateManager* GameState = GetWorld()->GetGameState<AGameStateManager>();
	GameStateManager = GetWorld()->GetGameState<AGameStateManager>();

	GameStateManager->SetSpawnParameters();
}

void AMasterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMasterPlayerController::InitializeLevelLoad()
{
	GameStateManager->InitializeBackgroundLevelLoad();
}