// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#pragma comment(lib, "")

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "AssetBundleData.h"
#include "SubclassOf.h"

#include "LocalSpawnableObject.generated.h"

UENUM(BlueprintType)
enum class EObjectClass : uint8
{
	OBCL_NONE	UMETA(DisplayName = "None"),
	OBCL_SHIP	UMETA(DisplayName = "Ship")
};

UCLASS(Blueprintable)
class CUPCAKESINSPACE_API ALocalSpawnableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ALocalSpawnableObject();

	//UPROPERTY(EditAnywhere, AssetRegistrySearchable, meta = (AssetBundles = "Ship"))
	UPROPERTY(EditAnywhere, AssetRegistrySearchable)
	EObjectClass ObjectClass;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	FPrimaryAssetId GetPrimaryAssetId() const override;
	void PostLoad();

#if WITH_EDITORONLY_DATA
	//void Serialize(FArchive& Ar) override;
	//void UpdateAssetBundleData();

	/*Refresh asset bundles prior to saving*/
	//void PreSave(const class ITargetPlatform* TargetPlatform) override;

protected:
	//FAssetBundleData AssetBundleData;
#endif

private:
	UPROPERTY(AssetRegistrySearchable)
	TSubclassOf<ALocalSpawnableObject> NativeClass;
};
