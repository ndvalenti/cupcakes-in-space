// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticUtilities.h"

FInt64Vector UStaticUtilities::_CurrentGlobalOffset = FInt64Vector::FInt64Vector(0, 0, 0);
float UStaticUtilities::_FarCheckDistance = 0.01f;
int UStaticUtilities::_SpawnCheckDistance = 2000000;

/*Not Currently Used*/
int UStaticUtilities::_SpawnDangerDistance = 20000000;

const FORCEINLINE FInt64Vector UStaticUtilities::CurrentGlobalOffset()
{
	return _CurrentGlobalOffset;
}

const FORCEINLINE float UStaticUtilities::FarCheckDistance()
{
	return _FarCheckDistance;
}

const FORCEINLINE int UStaticUtilities::SpawnCheckDistance()
{
	return _SpawnCheckDistance;
}

const FORCEINLINE void UStaticUtilities::SetGlobalOffset(const FInt64Vector& NewOffset)
{
	_CurrentGlobalOffset = NewOffset;
}

const FORCEINLINE void UStaticUtilities::TranslateGlobalOffset(const FIntVector& Translation)
{
	_CurrentGlobalOffset += Translation;
}