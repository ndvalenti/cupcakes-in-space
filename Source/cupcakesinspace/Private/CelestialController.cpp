// Fill out your copyright notice in the Description page of Project Settings.

#include "CelestialController.h"
#include "GameStateManager.h"
#include "cmath"

#include "EngineGlobals.h"
#include "Engine/Engine.h"


ACelestialController::ACelestialController()
{
	PrimaryActorTick.bCanEverTick = true;
	bAllowTickBeforeBeginPlay = false;

}

void ACelestialController::BeginPlay()
{
	Super::BeginPlay();
	SetTickGroup(TG_PostUpdateWork);
	//AbstractLocation = GetActorLocation();
	CurrentCamera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	GetWorld()->GetGameState<AGameStateManager>()->Celestials.Emplace(this);
}

void ACelestialController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	int32 PlayerIndex = GetWorld()->GetGameState<AGameStateManager>()->PlayerShipIndex;
	FInt64Vector PlayerLoc = GetWorld()->GetGameState<AGameStateManager>()->GetPlayerShipPreciseLocation();
	*/
	if (CurrentCamera)
	{
		UpdateScaleRatio();

		SetActorLocation(CurrentCamera->GetActorLocation() + AbstractLocation);
		//SetActorLocation(AbstractLocation);
	}
}

void ACelestialController::UpdateScaleRatio()
{
	//int64 Distance = FInt64Vector::Distance(UStaticUtilities::CurrentGlobalOffset(), TrueLocation);
	FInt64Vector DistanceVector = TrueLocation - UStaticUtilities::CurrentGlobalOffset();
	double Distance = DistanceVector.Magnitude();
	ScaleRatio = (log(Distance) / Distance) * 2500.f; /*Ew magic number*/
	FVector NewAbstract(DistanceVector.X * ScaleRatio, DistanceVector.Y * ScaleRatio, DistanceVector.Z * ScaleRatio);
	AbstractLocation = NewAbstract;
	NewAbstract = FVector::FVector((double)Size * ScaleRatio);
	if (NewAbstract.ContainsNaN())
	{
		NewAbstract = FVector::FVector(0, 0, 0);
	}
	this->SetActorScale3D(NewAbstract);
	//AbstractLocation.X = TrueLocation.X * ScaleRatio;
	//AbstractLocation.Y = TrueLocation.Y * ScaleRatio;
	//AbstractLocation.Z = TrueLocation.Z * ScaleRatio;

}
