// Fill out your copyright notice in the Description page of Project Settings.
#include "MasterPlayerController.h"
#include "Engine/AssetManager.h"
#include "LocalSpawnableObject.h"


void AMasterPlayerController::BeginPlay()
{
	UStaticUtilities::ResetGlobalOffset();
	Super::BeginPlay();

	bEnableClickEvents = true;
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;

	GameStateManager = GetWorld()->GetGameState<AGameStateManager>();
	ShipPrimaryAssetType = FName(TEXT("Ship"));
	UAssetManager& AssetManager = UAssetManager::Get();
	AssetManager.GetPrimaryAssetPathList(ShipPrimaryAssetType, ShipLoadableAssetList);

}

void AMasterPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMasterPlayerController::SpawnAllAtLocation()
{
	if (GameStateManager)
	{
		for (auto& Ship : GameStateManager->ShipMasterList)
		{
			if (Ship.Value->ShipDistanceGroup == EShipDistance::SH_LOCAL)
			{
				UE_LOG(LogTemp, Warning, TEXT("Preparing ship to spawn %s"), *Ship.Value->GetFullName());
				SpawnShip(Ship.Value);
			}
		}
	}
}

bool AMasterPlayerController::SpawnShip(UShipObject* Ship)
{	
	/**DEBUG LOOP - Print all resolved assets**/
	for (FSoftObjectPath& Asset : ShipLoadableAssetList)
	{
		if (Asset.ResolveObject())
		{
			UE_LOG(LogTemp, Warning, TEXT("Preloaded Asset Found: %s"), *Asset.GetAssetName());
		}
	}

	FSpawnPackage Spawn;
	for (FSoftObjectPath& Path : ShipLoadableAssetList)
	{
		if(*Path.GetAssetName() == Ship->ShipType)
		{
			UE_LOG(LogTemp, Warning, TEXT("Asset Ready to load, preparing spawnpackage: %s"), *Path.GetAssetName());

			Spawn.AssetPath = Path;
			Ship->GetSpawnPositionAndRotation(Spawn.Location, Spawn.Rotation);
			Streamable.RequestAsyncLoad(Spawn.AssetPath, FStreamableDelegate::CreateUObject(this, &AMasterPlayerController::SpawnAssetInWorld, Spawn));
			
			return true;
		}
	}
	return false;
}

void AMasterPlayerController::SpawnAssetInWorld(FSpawnPackage SpawnPackage)
{
	UE_LOG(LogTemp, Warning, TEXT("Asset loaded in memory, delegate triggered"));

	TSoftClassPtr<ALocalSpawnableObject> SoftClassPtr(SpawnPackage.AssetPath);

	LoadedLevels = GetWorld()->GetLevels();
	FActorSpawnParameters ActorSpawnParameters;

	for (ULevel* Level : LoadedLevels)
	{
		if (Level->GetOuter()->GetName() == FString(TEXT("LocalGrid")))
		{
			ActorSpawnParameters.OverrideLevel = Level;
			UE_LOG(LogTemp, Warning, TEXT("Level to spawn asset has been located"));
			break;
		}
	}

	if (SoftClassPtr)
	{
		//GetWorld()->SpawnActor<ALocalSpawnableObject>(Cast<UBlueprint>(Asset)->GeneratedClass, AssetPack.Location, AssetPack.Rotation);
		GetWorld()->SpawnActor<ALocalSpawnableObject>(SoftClassPtr.Get(), SpawnPackage.Location, SpawnPackage.Rotation, ActorSpawnParameters);
		UE_LOG(LogTemp, Warning, TEXT("Spawning Complete %s"), *SoftClassPtr.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Asset failed to load in time, may have been skipped"));
	}
}