// Fill out your copyright notice in the Description page of Project Settings.
#include "GameStateManager.h"
#include "CelestialController.h"
#include "LocalSpawnableObject.h"
#include "ExternalCamera.h"

/*--------------------------PUBLIC FUNCTIONS--------------------------*/
AGameStateManager::AGameStateManager() : _ShipIndex(0), PlayerShipIndex(-1)
{
	_BridgeLevel = FName("Bridge_Plain");
	_SystemLevel = FName("System_Plain");
	_LocalLevel = FName("LocalGrid");
}

void AGameStateManager::PopulateShipData()
{
	/*
	for (int i = 0; i < 2000; i++)
	{
		CreateNewShip(FString(TEXT("CrabBP_C")), FInt64Vector::FInt64Vector(0, i*10000, 0), _SystemLevel);
	}
	*/

	FInt64Vector PlayerSpawn(FInt64Vector::FInt64Vector(-643752100321, -1372058562733, 0) - FInt64Vector::FInt64Vector(1800000000, 1200000000, -500000000));
	FInt64Vector PlayerTarget(FInt64Vector::FInt64Vector(141569252166, -1078668714713, -23049890318) - FInt64Vector::FInt64Vector(2400000000, -1500000000, -500000000));
	CreateNewShip(FString(TEXT("CrabBP_C")), PlayerTarget + FInt64Vector::FInt64Vector(20000, 10000, 0), _SystemLevel);
	CreateNewShip(FString(TEXT("CrabBP_C")), PlayerTarget + FInt64Vector::FInt64Vector(-5000, 0, -20000), _SystemLevel);
	CreateNewShip(FString(TEXT("CrabBP_C")), PlayerTarget + FInt64Vector::FInt64Vector(0, 10000, -10000), _SystemLevel);
	CreateNewShip(FString(TEXT("CrabBP_C")), PlayerSpawn + FInt64Vector::FInt64Vector(0, 20000, 0), _SystemLevel);
	CreateNewShip(FString(TEXT("CrabBP_C")), PlayerSpawn + FInt64Vector::FInt64Vector(0, 0, 20000), _SystemLevel);
	SetAsPlayerShip(CreateNewShip(FString(TEXT("SwallowBP_C")), PlayerSpawn, _SystemLevel));

	/*DEBUG VECTOR*/
	FInt64Vector Offset = UStaticUtilities::CurrentGlobalOffset();
	UE_LOG(LogTemp, Warning, TEXT("Current Origin: %s"), *Offset.ToString());
}

FName AGameStateManager::GetBridgeLevel() const
{
	return _BridgeLevel;
}

FName AGameStateManager::GetSystemLevel() const
{
	return _SystemLevel;
}

FName AGameStateManager::GetLocalLevel() const
{
	return _LocalLevel;
}

void AGameStateManager::InitializeBackgroundLevelLoad()
{
	UpdateExternalLevel();
	SpawnLocalShips();
	SpawnPlayerShip();
}

bool AGameStateManager::PlayerShipExists() const
{
	return (PlayerShipIndex == -1 ? false : true);
}

void AGameStateManager::SortAllShipDistanceGroups()
{
	EShipDistance TargetDist;

	for (auto& Ship : ShipMasterList)
	{
		TargetDist = CalculateTargetDistanceGroup(Ship.Value);
		MoveShipToGroup(Ship.Key, Ship.Value->GetDistanceGroup(), TargetDist);
	}
}

void AGameStateManager::MoveShipToGroup(const int32& ShipIndex, const EShipDistance From, const EShipDistance To)
{
	if (From != To)
	{
		/* We don't want to move the player to the the playership group unless it is already set as the current player ship */
		if (To != EShipDistance::SH_PLAYER || ShipIndex == PlayerShipIndex)
		{
			AddShipToGroup(ShipIndex, To);
			ShipMasterList.FindRef(ShipIndex)->SetDistanceGroup(To);
			RemoveShipFromGroup(ShipIndex, From);
		}
	}
}

int32 AGameStateManager::CreateNewShip(const FString& ShipType, const FInt64Vector& ShipLocation, const FName& System)
{
	UShipObject* TempShip;
	TempShip = NewObject<UShipObject>();
	int32 ShipIndex = _ShipIndex;

	ShipMasterList.Add(ShipIndex, TempShip);
	EShipDistance TempDist;

	TempShip->InitializeShip(ShipType, ShipLocation, ShipIndex, System);
	TempDist = UpdateShipDistanceGroup(TempShip, System);
	AddShipToGroup(ShipIndex, TempDist);

	_ShipIndex++;
	return ShipIndex;
}

void AGameStateManager::SetAsPlayerShip(const int32& ShipIndex)
{
	UShipObject* PlayerShip;
	PlayerShip = ShipMasterList.FindRef(ShipIndex);

	if (PlayerShipExists())
	{
		int32 OldPlayerIndex = PlayerShipIndex;
		PlayerShipIndex = ShipIndex;

		UShipObject* OldPlayerShip = ShipMasterList.FindRef(OldPlayerIndex);
		OldPlayerShip->SetDistanceGroup(EShipDistance::SH_EMPTY);
		EShipDistance DistanceGroup = OldPlayerShip->CalculateNewDistanceGroup(this);

		MoveShipToGroup(OldPlayerIndex, EShipDistance::SH_EMPTY, DistanceGroup);

		/* If the old player ship is spawned we have to move it to the general spawned list */
		if (SpawnedPlayerShip != nullptr)
		{
			SpawnedShips.Add(SpawnedPlayerShip);
		}
	}
	else
	{
		PlayerShipIndex = ShipIndex;
	}

	MoveShipToGroup(ShipIndex, PlayerShip->GetDistanceGroup(), EShipDistance::SH_PLAYER);
	
	FInt64Vector NewOffset;
	PlayerShip->GetPreciseLocation(NewOffset);
	SetAndFocusGlobalOffset(NewOffset);

	SortAllShipDistanceGroups();
	ALocalSpawnableObject* SpawnedToRemove;
	for (auto& Ships : SpawnedShips)
	{
		if (Ships->GetShipIndex() == ShipIndex)
		{
			SpawnedToRemove = Ships;
			SpawnedPlayerShip = Ships;
			break;
		}
	}
	SpawnedShips.RemoveSingle(SpawnedToRemove);
	/*TODO Add logic to this area or above function to spawn ships based on the distance groups at this point*/
}

void AGameStateManager::SetAndFocusGlobalOffset(const FInt64Vector& Offset)
{
	UStaticUtilities::SetGlobalOffset(Offset);
}

UShipObject* AGameStateManager::GetPlayerShipObject()
{
	return ShipMasterList.FindRef(PlayerShipIndex);
}

void AGameStateManager::SetSpawnParameters()
{
	UAssetManager& AssetManager = UAssetManager::Get();
	ShipPrimaryAssetType = FName(TEXT("Ship"));
	AssetManager.GetPrimaryAssetPathList(ShipPrimaryAssetType, ShipLoadableAssetList);
}

bool AGameStateManager::SpawnShip(UShipObject* Ship)
{
	if (Ship->GetDistanceGroup() == EShipDistance::SH_LOCAL || Ship->GetDistanceGroup() == EShipDistance::SH_PLAYER)
	{
		FSpawnPackage Spawn;
		FString ShipType;
		for (FSoftObjectPath& Path : ShipLoadableAssetList)
		{
			Ship->GetShipType(ShipType);
			if(*Path.GetAssetName() == ShipType)
			{
				Spawn.AssetPath = Path;
				Ship->GetShipIndex(Spawn.Index);
				Ship->GetSpawnPositionAndRotation(Spawn.Location, Spawn.Rotation);
				Streamable.RequestAsyncLoad(Spawn.AssetPath, FStreamableDelegate::CreateUObject(this, &AGameStateManager::SpawnAssetInWorld, Spawn, Ship));
				return true;
			}
		}
	}
	return false;
}

void AGameStateManager::DespawnShip(ALocalSpawnableObject* Ship)
{
	UShipObject* ShipObject;
	int32 DespawnIndex = Ship->GetShipIndex();
	ShipObject = ShipMasterList.FindRef(DespawnIndex);

	/* Separate handling for despawning the player ship */
	if (SpawnedPlayerShip == Ship)
	{
		UE_LOG(LogTemp, Error, TEXT("player ship found"));
		SpawnedPlayerShip = nullptr;
	}
	else
	{
		SpawnedShips.RemoveSingle(Ship);
	}

	if (Ship->DespawnShip())
	{
		/**TODO if ship is to be removed completely from world, possibly in calculatenewdistancegroup--possibly add a marked for destruction bool to shipobject or create a different function*/
		ShipObject->bSpawned = false;
		ShipObject->CalculateNewDistanceGroup(this);
	}
}

void AGameStateManager::SpawnLocalShips()
{
	UShipObject* ShipRef;
	/* Loop through the local ships and spawn them all */
	for (int32& Ship : LocalShips)
	{
		ShipRef = ShipMasterList.FindRef(Ship);
		if (!ShipRef->bSpawned)
		{
			SpawnShip(ShipRef);
		}
	}
}

void AGameStateManager::SpawnPlayerShip()
{
	UShipObject* ShipRef;
	ShipRef = GetPlayerShipObject();
	if (!ShipRef->bSpawned)
	{
		SpawnShip(ShipRef);
	}
	//AExternalCamera* ExternalCamera = Cast<AExternalCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	//ExternalCamera->SetNewCameraFocus();
}

void AGameStateManager::LoadShipsInArea()
{

}

void AGameStateManager::WarpShip(ALocalSpawnableObject* LocalSpawnedShip, const FInt64Vector& Destination)
{
	/* Only move the scene if the player ship is warping */
	if (SpawnedPlayerShip == LocalSpawnedShip)
	{
		/* Unload all local ships */
		for (auto& Ship : SpawnedShips)
		{
			Ship->DespawnShip();
		}

		/* Move offset */
		UStaticUtilities::SetGlobalOffset(Destination);
		SortAllShipDistanceGroups();

		/* Move player ship */
		WarpShip(LocalSpawnedShip->GetShipIndex(), Destination);

		/* Spawn ships in new location */
		SpawnLocalShips();

		/*
		* Future:
		* 	Make a list of ships that are currently spawned and compare it against the ships to be loaded
		*	Load new ships in advance while retaining old ships
		*	Delay spawning of new ships until player ship hits new grid
		*	Center camera focus on player ship
		*	Needs some delegate or something from ShipObject to figure out when warping needs to end
		*/
	}
	else
	{
		/* Warp this nonplayer ship */
		WarpShip(LocalSpawnedShip->GetShipIndex(), Destination);

		/* return control to ShipObject */
	}
}

void AGameStateManager::WarpShip(const int32 Index, const FInt64Vector& Destination)
{
	TeleportShip(Index, Destination);
}

void AGameStateManager::TeleportShip(const int32 ShipIndex, const FInt64Vector& Destination)
{
	ShipMasterList.FindRef(ShipIndex)->TeleportToPreciseLocation(Destination);
}

void AGameStateManager::SpawnAssetInWorld(FSpawnPackage SpawnPackage, UShipObject * Ship)
{
	TSoftClassPtr<ALocalSpawnableObject> SoftClassPtr(SpawnPackage.AssetPath);

	if (SoftClassPtr)
	{
		/*TODO add proper error handling if the ship cannot spawn for some reason*/
		ALocalSpawnableObject* LocalShip = GetWorld()->SpawnActor<ALocalSpawnableObject>(SoftClassPtr.Get(), SpawnPackage.Location, SpawnPackage.Rotation, ExternalSpawnParameters);
		Ship->bSpawned = true;
		LocalShip->SetShipIndex(SpawnPackage.Index);
		if (SpawnPackage.Index == PlayerShipIndex)
		{
			SpawnedPlayerShip = LocalShip;

			AExternalCamera* ExternalCamera = Cast<AExternalCamera>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
			ExternalCamera->SetNewCameraFocus(LocalShip);
		}
		else
		{
			SpawnedShips.Add(LocalShip);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Asset failed to load in time, may have been skipped"));
	}
}

void AGameStateManager::SetPossessedObject(AExternalCamera* In)
{
	
}

/*--------------------------PROTECTED FUNCTIONS--------------------------*/
EShipDistance AGameStateManager::UpdateShipDistanceGroup(UShipObject* Ship)
{
	FName System;
	Ship->GetShipSystem(System);

	EShipDistance ShipDistance;
	ShipDistance = CalculateTargetDistanceGroup(Ship, System);

	Ship->SetDistanceGroup(ShipDistance);
	return ShipDistance;
}

EShipDistance AGameStateManager::UpdateShipDistanceGroup(UShipObject* Ship, const FName& System)
{
	EShipDistance ShipDistance;
	ShipDistance = CalculateTargetDistanceGroup(Ship, System);

	Ship->SetDistanceGroup(ShipDistance);
	return ShipDistance;
}

EShipDistance AGameStateManager::CalculateTargetDistanceGroup(const UShipObject* Ship)
{
	FName System;
	Ship->GetShipSystem(System);

	if (PlayerShipExists())
	{
		FName PlayerSystem;
		ShipMasterList.FindRef(PlayerShipIndex)->GetShipSystem(PlayerSystem);
		// If they are in the same system
		if (System == PlayerSystem)
		{
			return Ship->CalculateNewDistanceGroup(this);
		}
	}
	return EShipDistance::SH_EXTERNAL;
}

EShipDistance AGameStateManager::CalculateTargetDistanceGroup(const UShipObject* Ship, const FName& System)
{
	if (PlayerShipExists())
	{
		FName PlayerSystem;
		ShipMasterList.FindRef(PlayerShipIndex)->GetShipSystem(PlayerSystem);
		// If they are in the same system
		if (System == PlayerSystem)
		{
			return Ship->CalculateNewDistanceGroup(this);
		}
	}
	return EShipDistance::SH_EXTERNAL;
}

void AGameStateManager::AddShipToGroup(const int32& ShipIndex, const EShipDistance To)
{
	/*TODO: add conditional to ensure ship is eligable to be sorted (ie different system)*/
	switch (To)
	{
	case EShipDistance::SH_EMPTY:
		//UnsortedShips.Emplace(ShipIndex);
		break;
	case EShipDistance::SH_EXTERNAL:
		OutSystemShips.Emplace(ShipIndex);
		break;
	case EShipDistance::SH_DOCKED:
		DockedShips.Emplace(ShipIndex);
		break;
	case EShipDistance::SH_PLAYER:
		ShipMasterList.FindRef(ShipIndex)->FlagAsPlayer();
		break;
	case EShipDistance::SH_FAR:
		FarShips.Emplace(ShipIndex);
		break;
	case EShipDistance::SH_NEAR:
		NearShips.Emplace(ShipIndex);
		break;
	case EShipDistance::SH_LOCAL:
		AddToLocal(ShipIndex);
		break;
	default:
		break;
	}
}

void AGameStateManager::RemoveShipFromGroup(const int32& ShipIndex, const EShipDistance From)
{
	switch (From)
	{
	case EShipDistance::SH_EMPTY:
		//UnsortedShips.RemoveSingle(ShipIndex);
		break;
	case EShipDistance::SH_EXTERNAL:
		OutSystemShips.RemoveSingle(ShipIndex);
		break;
	case EShipDistance::SH_DOCKED:
		DockedShips.RemoveSingle(ShipIndex);
		break;
	case EShipDistance::SH_PLAYER:
		break;
	case EShipDistance::SH_FAR:
		FarShips.RemoveSingle(ShipIndex);
		break;
	case EShipDistance::SH_NEAR:
		NearShips.RemoveSingle(ShipIndex);
		break;
	case EShipDistance::SH_LOCAL:
		RemoveFromLocal(ShipIndex);
		break;
	default:
		break;
	}
}

void AGameStateManager::AddToLocal(const int32& ShipIndex)
{
	LocalShips.Emplace(ShipIndex);
}

void AGameStateManager::RemoveFromLocal(const int32& ShipIndex)
{
	LocalShips.RemoveSingle(ShipIndex);
}

void AGameStateManager::UpdateExternalLevel()
{
	/*Get all currently loaded levels*/
	TArray<ULevel*> LoadedLevels;
	LoadedLevels = GetWorld()->GetLevels();

	for (ULevel* Level : LoadedLevels)
	{
		if (Level->GetOuter()->GetName() == _LocalLevel.ToString())
		{
			ExternalSpawnParameters.OverrideLevel = Level;
			break;
		}
	}
}