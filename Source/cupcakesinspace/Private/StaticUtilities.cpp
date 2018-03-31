// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticUtilities.h"

FInt64Vector UStaticUtilities::_CurrentGlobalOffset = FInt64Vector::FInt64Vector(0, 0, 0);
float UStaticUtilities::_FarCheckDistance = 0.01f;
int UStaticUtilities::_SpawnCheckDistance = 2000000;

/*Not Currently Used*/
int UStaticUtilities::_SpawnDangerDistance = 20000000;
/*
UStaticUtilities::UStaticUtilities(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{ }
*/
const FInt64Vector UStaticUtilities::CurrentGlobalOffset()
{
	return _CurrentGlobalOffset;
}

const float UStaticUtilities::FarCheckDistance()
{
	return _FarCheckDistance;
}

const int UStaticUtilities::SpawnCheckDistance()
{
	return _SpawnCheckDistance;
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
