// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticUtilities.h"

FInt64Vector UStaticUtilities::_CurrentGlobalOffset = FInt64Vector::FInt64Vector(0, 0, 0);
float UStaticUtilities::_FarCheckDistance = 0.01f;
/*Distance before an origin rebase is triggered in ue4 units (dm in game world)*/
int32 UStaticUtilities::_RebaseDistance = 300000;
/*Distance threshold for spawning an asset into the world*/
int32 UStaticUtilities::_SpawnCheckDistance = 350000;
int32 UStaticUtilities::_DespawnDistance = UStaticUtilities::_SpawnCheckDistance + UStaticUtilities::_RebaseDistance;

/*Not Currently Used*/
int32 UStaticUtilities::_SpawnDangerDistance = 20000000;

const FInt64Vector UStaticUtilities::CurrentGlobalOffset()
{
	return _CurrentGlobalOffset;
}

const float UStaticUtilities::FarCheckDistance()
{
	return _FarCheckDistance;
}

const int32 UStaticUtilities::SpawnCheckDistance()
{
	return _SpawnCheckDistance;
}

const int32 UStaticUtilities::RebaseDistance()
{
	return _RebaseDistance;
}

const int32 UStaticUtilities::DespawnDistance()
{
	return _DespawnDistance;
}

const void UStaticUtilities::SetGlobalOffset(const FInt64Vector& NewOffset)
{
	_CurrentGlobalOffset = NewOffset;
}

const void UStaticUtilities::TranslateGlobalOffset(const FIntVector& Translation)
{
	_CurrentGlobalOffset += Translation;
}

const void UStaticUtilities::ResetGlobalOffset()
{
	_CurrentGlobalOffset = FInt64Vector::FInt64Vector(0, 0, 0);
}

const FVector UStaticUtilities::ConvertPreciseToImpreciseDistance(const FInt64Vector& In)
{
	FVector Out;
	Out.X = (In.X / 1798754748);
	Out.Y = (In.Y / 1798754748);
	Out.Z = (In.Z / 1798754748);
	Out /= 100.f;
	return Out;
}
