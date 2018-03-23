// Fill out your copyright notice in the Description page of Project Settings.
#include "GameStateManager.h"
#include "LocalPawn.h"

AGameStateManager::AGameStateManager() : ShipIndex(0)
{
	// Technically this should hook the asset loading into our scan function
	// It doesn't work in practice and needs to be looked at again
	//FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	//AssetRegistryModule.Get().OnFilesLoaded().AddUObject(this, &AGameStateManager::OnFilesLoaded);
	OnFilesLoaded();
	BuildList();
}

void AGameStateManager::BuildList()
{
	FString Name;
	FInt64Vector Position;
	UShipObject* TempShip;

	TempShip = NewObject<UShipObject>();
	Name = FString(TEXT("CrabBP"));
	TempShip->InitializeShip(Name, FInt64Vector::FInt64Vector(-4568,10151,88455));
	ShipMasterList.Add(++ShipIndex, TempShip);
	
	TempShip = NewObject<UShipObject>();
	Name = FString(TEXT("SwallowBP"));
	TempShip->InitializeShip(Name, FInt64Vector::FInt64Vector(498, 8680, -1056));
	ShipMasterList.Add(++ShipIndex, TempShip);

	TempShip = NewObject<UShipObject>();
	Name = FString(TEXT("CrabBP"));
	TempShip->InitializeShip(Name, FInt64Vector::FInt64Vector(-11458, 100151, 8800455));
	ShipMasterList.Add(++ShipIndex, TempShip);

	TempShip = NewObject<UShipObject>();
	Name = FString(TEXT("SwallowBP"));
	TempShip->InitializeShip(Name, FInt64Vector::FInt64Vector(52360000000000, 0, 0));
	ShipMasterList.Add(++ShipIndex, TempShip);
	
}

void AGameStateManager::OnFilesLoaded()
{
	//UE_LOG(LogTemp, Warning, TEXT("OnFilesLoaded start"));
	// https://docs.unrealengine.com/en-us/Programming/Assets/Registry
	// http://kantandev.com/articles/finding-all-classes-blueprints-with-a-given-base
	// https://wiki.unrealengine.com/TAssetPtr_and_Asynchronous_Asset_Loading
	// WARNING: Some UE4 Documentation out of date as of 2018/3/12

	// The class we'll be finding subclasses for; to be supplied when made modular in the future
	TSubclassOf<UObject> ParentClass = ALocalPawn::StaticClass();

	// Load the asset registry module
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetList;
	FARFilter Filter;
	const FName ClassName = ParentClass->GetFName();

	
	// Runs synch scan on assets to ensure they are all loaded
	// This is dumb and should be done better
	// This code may not work without this
	// It can be updated to only synch scan a single directory
	// See asynchronous data gathering in UE4 wiki
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	TArray<FString> ContentPaths;
	ContentPaths.Add(TEXT("/Game"));
	AssetRegistry.ScanPathsSynchronous(ContentPaths);
	

	// Define filter settings; tags to be made parameters in function call
	Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Filter.bRecursiveClasses = true;
	Filter.bRecursivePaths = true;
	Filter.TagsAndValues.Add(TEXT("ObjectClass"), TEXT("OBCL_SHIP"));

	AssetRegistryModule.Get().GetAssets(Filter, AssetList);
	
	for (const FAssetData& Asset : AssetList)
	{
		ShipAssetList.Add(Asset.ToSoftObjectPath());
	}

	/* Testing Asset numbers


	for (const FSoftObjectPath& Path : ShipAssetList)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset Registered: %s"), *Path.GetAssetName())
	}
	*/
	UE_LOG(LogTemp, Warning, TEXT("%d Assets"), AssetList.Num());
	UE_LOG(LogTemp, Warning, TEXT("%d Ship blueprint assets loaded into gamemanager"), ShipAssetList.Num());
	
}
