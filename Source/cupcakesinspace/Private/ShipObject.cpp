// Fill out your copyright notice in the Description page of Project Settings.
#include "ShipObject.h"
#include "GameStateManager.h"

/*--------------------------PROTECTED FUNCTIONS--------------------------*/
UShipObject::UShipObject() : bSpawned(false)
{

}

void UShipObject::InitializeShip(const FString& Type, const FInt64Vector& InitialPosition, const int32& ShipIndex, const FName& System)
{
	_ShipType = Type;
	_PreciseShipLocation = InitialPosition;
	_ShipIndex = ShipIndex;
	_System = System;
	UpdateImpreciseLocation();
}

void UShipObject::SetDistanceGroup(const EShipDistance ShipDistance)
{
	_ShipDistanceGroup = ShipDistance;
}

void UShipObject::FlagAsPlayer()
{
	_ShipDistanceGroup = EShipDistance::SH_PLAYER;
}

void UShipObject::UpdateImpreciseLocation()
{
	_LongShipLocation.X = (_PreciseShipLocation.X / 1798754748);
	_LongShipLocation.Y = (_PreciseShipLocation.Y / 1798754748);
	_LongShipLocation.Z = (_PreciseShipLocation.Z / 1798754748);
	_LongShipLocation /= 100.f;
}

void UShipObject::SetSystem(FName& In)
{
	_System = In;
}

/*--------------------------PUBLIC FUNCTIONS--------------------------*/
void UShipObject::GetDistanceGroup(EShipDistance& Out) const
{
	Out = _ShipDistanceGroup;
}

EShipDistance UShipObject::GetDistanceGroup() const
{
	return _ShipDistanceGroup;
}

void UShipObject::GetSpawnPositionAndRotation(FVector &OutVector, FRotator &OutRotation) const
{
	/*
	* Dividing these large floats won't be completely accurate but having a ship's model
	* possibly spawn at most a few cm (mm in game space) in the wrong direction should go unnoticed
	* due to the sheer scale of this project and the fact that the local space takes over a ship's logic
	* once it is close enough to spawn
	*
	* Once a final implementation is decided upon this function will be further documented in the header file
	*/
	OutVector.X = (float)(_PreciseShipLocation.X - UStaticUtilities::CurrentGlobalOffset().X);
	OutVector.Y = (float)(_PreciseShipLocation.Y - UStaticUtilities::CurrentGlobalOffset().Y);
	OutVector.Z = (float)(_PreciseShipLocation.Z - UStaticUtilities::CurrentGlobalOffset().Z);
	//OutVector /= 10.f;
	OutRotation = FRotator::ZeroRotator;
}

EShipDistance UShipObject::CalculateNewDistanceGroup(AGameStateManager* GameStateManager, bool bPlayerExists) const
{
	// This function assumes that the ship in question is in the same system as the player
	if (bPlayerExists)
	{
		FInt64Vector PlayerLocation = UStaticUtilities::CurrentGlobalOffset();
		if (_ShipDistanceGroup != EShipDistance::SH_PLAYER && _ShipDistanceGroup != EShipDistance::SH_DOCKED)
		{
			if (FVector::Dist(_LongShipLocation, UStaticUtilities::ConvertPreciseToImpreciseDistance(PlayerLocation)) >= UStaticUtilities::FarCheckDistance())
			{
				return EShipDistance::SH_FAR;
			}
			else
			{
				if (FInt64Vector::Distance(PlayerLocation, _PreciseShipLocation) >= UStaticUtilities::SpawnCheckDistance())
				{
					return EShipDistance::SH_NEAR;
				}
				else
				{
					return EShipDistance::SH_LOCAL;
				}
			}
		}
		// This ship's distance group will not change
		return _ShipDistanceGroup;
	}
	return EShipDistance::SH_EXTERNAL;
}

void UShipObject::GetPreciseLocation(FInt64Vector& Out) const
{
	Out = _PreciseShipLocation;
}

void UShipObject::GetShipIndex(int32& Out) const
{
	Out = _ShipIndex;
}

void UShipObject::GetImpreciseLocation(FVector& Out) const
{
	Out = _LongShipLocation;
}

void UShipObject::GetShipType(FString& Out) const
{
	Out = _ShipType;
}

void UShipObject::GetShipSystem(FName& Out) const
{
	Out = _System;
}

FString UShipObject::GetDisplayDistanceTo(const FInt64Vector& From) const
{
	/*Maybe a good idea*/
	//UpdateFarLocation();

	float Distance = FVector::Dist(_LongShipLocation, UStaticUtilities::ConvertPreciseToImpreciseDistance(From));
	if (Distance >= 0.01f)
	{
		return GetFloatAsString(Distance, 2).Append(FString(TEXT(" lm")));
	}
	else
	{
		Distance = FInt64Vector::Distance(From, _PreciseShipLocation);
		if (Distance > 10000)
		{
			Distance /= 100000;
			return GetFloatAsString(Distance,  2).Append(FString(TEXT(" km")));
		}
		else
		{
			Distance /= 100;
			return GetFloatAsString(Distance, 0).Append(FString(TEXT(" m")));
		}
	}
}

FString UShipObject::GetFloatAsString(float Number, const int32 Precision) const
{
	float Rounded = roundf(Number);
	if (FMath::Abs(Number - Rounded) < FMath::Pow(10, -1 * Precision))
	{
		Number = Rounded;
	}
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = 1;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	return FText::AsNumber(Number, &NumberFormat).ToString();
}

void UShipObject::TeleportToPreciseLocation(const FInt64Vector& Destination)
{
	_PreciseShipLocation = Destination;
}
