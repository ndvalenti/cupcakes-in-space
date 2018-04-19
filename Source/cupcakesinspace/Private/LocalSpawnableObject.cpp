// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalSpawnableObject.h"


ALocalSpawnableObject::ALocalSpawnableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	NativeClass = GetClass();
}

bool ALocalSpawnableObject::DespawnShip()
{
	return Destroy();
}

void ALocalSpawnableObject::BeginPlay()
{
	Super::BeginPlay();

}

void ALocalSpawnableObject::WarpTo(const FInt64Vector& Destination)
{
	AGameStateManager* GameStateManager = GetWorld()->GetGameState<AGameStateManager>();

	/*Tell the GameStateManager to warp this ship*/
	if (GameStateManager != nullptr)
	{
		GameStateManager->WarpShip(this, Destination);
	}
}

void ALocalSpawnableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FPrimaryAssetId ALocalSpawnableObject::GetPrimaryAssetId() const
{
	FPrimaryAssetId PrimaryAssetId;
	PrimaryAssetId.PrimaryAssetType = FName(TEXT("Ship"));
	PrimaryAssetId.PrimaryAssetName = FPrimaryAssetType(GetClass()->GetFName());
	return PrimaryAssetId;
}

void ALocalSpawnableObject::PostLoad()
{
	Super::PostLoad();

	//UAssetManager::Get().RefreshAssetData(this);
	
#if WITH_EDITORONLY_DATA
	UAssetManager::Get().RefreshAssetData(this);
	
#endif
}

void ALocalSpawnableObject::SetShipIndex(int32 IndexIn)
{
	_ShipIndex = IndexIn;
}

int32 ALocalSpawnableObject::GetShipIndex() const
{
	return _ShipIndex;
}
