// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Object.h"
#include "Int64Vector.h"

#include "StaticUtilities.generated.h"

UCLASS()
class CUPCAKESINSPACE_API UStaticUtilities : public UObject
{
	GENERATED_BODY()
public:
	//UStaticUtilities(const FObjectInitializer& ObjectInitializer);

protected:
	static FInt64Vector _CurrentGlobalOffset;
	static float _FarCheckDistance;
	static int _SpawnCheckDistance;

	/*Not Currently Used*/
	static int _SpawnDangerDistance;

public:
	static const FInt64Vector CurrentGlobalOffset();
	static const void SetGlobalOffset(const FInt64Vector& NewOffset);
	static const void TranslateGlobalOffset(const FIntVector& Translation);
	static const int SpawnCheckDistance();
	static const float FarCheckDistance();
	static const void ResetGlobalOffset();
};