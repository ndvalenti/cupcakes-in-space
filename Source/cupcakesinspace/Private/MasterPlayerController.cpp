// Fill out your copyright notice in the Description page of Project Settings.
#include "MasterPlayerController.h"
#include "LocalPawn.h"

void AMasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bEnableClickEvents = true;
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;

	GameStateManager = GetWorld()->GetGameState<AGameStateManager>();
}

void AMasterPlayerController::Tick(float DeltaTime)
{
	
}

void AMasterPlayerController::SpawnAllAtLocation()
{
	if (GameStateManager)
	{
		/*
		for (UShipObject* Ship : GameStateManager->LocalShips)
		{
			SpawnShip(Ship);
		}
		*/
		for (auto& Ship : GameStateManager->ShipMasterList)
		{
			if (Ship.Value->ShipDistanceGroup == EShipDistance::SH_LOCAL)
			{
				SpawnShip(Ship.Value);
			}
		}
	}
}

bool AMasterPlayerController::SpawnShip(UShipObject* Ship)
{
	//UE_LOG(LogTemp, Warning, TEXT("Close Distance: %i"), FInt64Vector::Distance(Ship->GetCloseLocation(), CurrentGlobalOffset))
	//UE_LOG(LogTemp, Warning, TEXT("Far Distance: %f"), FVector::Dist(Ship->GetFarLocation(), Ship->ConvertCloseToFarLocation(CurrentGlobalOffset)))
	//UE_LOG(LogTemp, Warning, TEXT("Display Distance: %s"), *Ship->GetDisplayDistanceTo(CurrentGlobalOffset))
	
	FSpawnPackage Spawn;
	for (FSoftObjectPath& Path : GameStateManager->ShipAssetList)
	{
		if (*Path.GetAssetName() == Ship->ShipType)
		{
			UE_LOG(LogTemp, Warning, TEXT("Loading Asset"));
			Spawn.AssetPath = Path;
			Ship->GetSpawnPositionAndRotation(Spawn.Location, Spawn.Rotation);
			SpawnPackage.Enqueue(Spawn);
			Streamable.RequestAsyncLoad(Spawn.AssetPath, FStreamableDelegate::CreateUObject(this, &AMasterPlayerController::SpawnAsset));
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed to discover asset %s"), *Ship->ShipType)
	return false;
	
	//return true;
}

void AMasterPlayerController::Test()
{
	/*
	if (GameStateManager){
		FSpawnPackage Spawn;
		FSoftObjectPath temp;

		GameStateManager->TestShip = NewObject<UShipObject>();

		for (const FSoftObjectPath& Path : GameStateManager->ShipAssetList)
		{
			if (*Path.GetAssetName() == GameStateManager->TestShip->ShipType) {
				temp = Path;
				break;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Loading Asset"));
		
		Spawn.AssetPath = temp;
		GameStateManager->TestShip->GetSpawnPositionAndRotation(Spawn.Location, Spawn.Rotation, CurrentGlobalOffset);
		SpawnPackage.Enqueue(Spawn);

		Streamable.RequestAsyncLoad(temp, FStreamableDelegate::CreateUObject(this, &AMasterPlayerController::SpawnAsset));
	}
	*/
}

void AMasterPlayerController::SpawnAsset()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawing Asset"));
	UObject* Asset;
	FSpawnPackage AssetPack;

	SpawnPackage.Dequeue(AssetPack);
	Asset = AssetPack.AssetPath.ResolveObject();

	LoadedLevels = GetWorld()->GetLevels();
	FActorSpawnParameters Parameters;

	for (ULevel* Level : LoadedLevels)
	{
		if (Level->GetOuter()->GetName() == FString(TEXT("LocalGrid")))
		{
			Parameters.OverrideLevel = Level;
			break;
		}
	}

	/*Add proper assertions*/
	if (Asset)
	{
		GetWorld()->SpawnActor<ALocalPawn>(Cast<UBlueprint>(Asset)->GeneratedClass, AssetPack.Location, AssetPack.Rotation, Parameters);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Asset failed to load in time, may have been skipped"));
	}
}

void AMasterPlayerController::SetCameraFocus(AActor *FocusedPawn)
{
	CameraFocus = FocusedPawn;
	UE_LOG(LogTemp, Warning, TEXT("Camera Possessed: %s"), *CameraFocus->GetName());
}