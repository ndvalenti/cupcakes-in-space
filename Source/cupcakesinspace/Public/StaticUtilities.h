// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Object.h"
#include "Int64Vector.h"

#include "StaticUtilities.generated.h"


// To Add:
// - Static references to player ship
// - Timer Functions

UCLASS()
class CUPCAKESINSPACE_API UStaticUtilities : public UObject
{
	GENERATED_BODY()

protected:
	static FInt64Vector _CurrentGlobalOffset;
	static float _FarCheckDistance;
	static int32 _SpawnCheckDistance;
	static int32 _RebaseDistance;
	static int32 _DespawnDistance;

	/*Not Currently Used*/
	static int32 _SpawnDangerDistance;

public:
	static const void SetGlobalOffset(const FInt64Vector& NewOffset);
	static const void TranslateGlobalOffset(const FIntVector& Translation);

	static const FInt64Vector CurrentGlobalOffset();
	static const float FarCheckDistance();
	static const int32 SpawnCheckDistance();
	static const int32 RebaseDistance();
	static const int32 DespawnDistance();

	static const void ResetGlobalOffset();
	static const FVector ConvertPreciseToImpreciseDistance(const FInt64Vector& In);

	/*DEBUG*/
	//UE_LOG(LogTemp, Warning, TEXT("Sorting Begin"));
	//int64 TockTime;
	//int64 TickTime = FPlatformTime::ToMilliseconds64(FPlatformTime::Cycles64());
	/**/

	/*DEBUG*/
	//TockTime = FPlatformTime::ToMilliseconds64(FPlatformTime::Cycles64()) - TickTime;
	//UE_LOG(LogTemp, Warning, TEXT("Sorting End: %lld ms"), TockTime);
	/**/
};