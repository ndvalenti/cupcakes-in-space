// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalSpawnableObject.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"
#include "Engine/AssetManager.h"


ALocalSpawnableObject::ALocalSpawnableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	NativeClass = GetClass();
}

void ALocalSpawnableObject::BeginPlay()
{
	Super::BeginPlay();
	
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
	UE_LOG(LogTemp, Warning, TEXT("Primary Asset Id Created: %s"), *PrimaryAssetId.ToString())
	return PrimaryAssetId;
}

void ALocalSpawnableObject::PostLoad()
{
	Super::PostLoad();
	
#if WITH_EDITORONLY_DATA
	/* We're not currently using bundles
	FAssetBundleData OldData = AssetBundleData;

	UpdateAssetBundleData();

	if (UAssetManager::IsValid() && OldData != AssetBundleData)
	{
		// Bundles changed, refresh
		UAssetManager::Get().RefreshAssetData(this);
	}
	*/
	UAssetManager::Get().RefreshAssetData(this);
	
#endif
}

/*
#if WITH_EDITORONLY_DATA
void ALocalSpawnableObject::Serialize(FArchive & Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading() && (Ar.UE4Ver() < VER_UE4_ADD_TRANSACTIONAL_TO_DATA_ASSETS))
	{
		SetFlags(RF_Transactional);
	}
}

void ALocalSpawnableObject::UpdateAssetBundleData()
{
	AssetBundleData.Reset();

	// By default parse the metadata
	if (UAssetManager::IsValid())
	{
		UAssetManager::Get().InitializeAssetBundlesFromMetadata(this, AssetBundleData);
	}
}

void ALocalSpawnableObject::PreSave(const ITargetPlatform * TargetPlatform)
{
#if WITH_EDITOR
	FCoreUObjectDelegates::OnObjectSaved.Broadcast(this);
#endif
	UpdateAssetBundleData();

	if (UAssetManager::IsValid())
	{
		// Bundles may have changed, refresh
		UAssetManager::Get().RefreshAssetData(this);
	}
}
#endif
*/
