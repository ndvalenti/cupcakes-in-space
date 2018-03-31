// Fill out your copyright notice in the Description page of Project Settings.
#include "ShipObject.h"
/*
 * Default Constructor
 */
UShipObject::UShipObject()
{

}

/* 
 * Standard constructor to create a ship 
 */
void UShipObject::InitializeShip(const FString& Type, const FInt64Vector& InitialPosition)
{
	ShipType = Type;
	PreciseShipLocation = InitialPosition;
	UpdateFarLocation();
	UE_LOG(LogTemp, Warning, TEXT("Ship %s initialized at %s:  %s away"), *ShipType, *PreciseShipLocation.ToString(), *GetDisplayDistanceTo(UStaticUtilities::CurrentGlobalOffset()))
	SetDistanceGroup();
}

FInt64Vector UShipObject::GetCloseLocation()
{
	return PreciseShipLocation;
}

FVector UShipObject::GetFarLocation()
{
	return LongShipLocation;
}

FString UShipObject::GetDisplayDistanceTo(const FInt64Vector& From)
{
	/*Maybe a good idea*/
	//UpdateFarLocation();

	float Distance = FVector::Dist(GetFarLocation(), ConvertCloseToFarLocation(From));
	if (Distance >= 0.01f)
	{
		return GetFloatAsString(Distance, 2).Append(FString(TEXT(" lm")));
	}
	else
	{
		Distance = FInt64Vector::Distance(From, GetCloseLocation());
		if (Distance > 10000)
		{
			Distance /= 10000;
			return GetFloatAsString(Distance,  2).Append(FString(TEXT(" km")));
		}
		else
		{
			Distance /= 10;
			return GetFloatAsString(Distance, 0).Append(FString(TEXT(" m")));
		}
	}
}

EShipDistance UShipObject::SetDistanceGroup()
{
	if (ShipDistanceGroup != EShipDistance::SH_PLAYER)
	{
		if (FVector::Dist(GetFarLocation(), ConvertCloseToFarLocation(UStaticUtilities::CurrentGlobalOffset())) >= UStaticUtilities::FarCheckDistance())
		{
			ShipDistanceGroup = EShipDistance::SH_FAR;
			
		}
		else
		{
			if (FInt64Vector::IntDistance(UStaticUtilities::CurrentGlobalOffset(), GetCloseLocation()) >= UStaticUtilities::SpawnCheckDistance())
			{
				ShipDistanceGroup = EShipDistance::SH_NEAR;
			}
			else
			{
				ShipDistanceGroup = EShipDistance::SH_LOCAL;
			}
		}
	}
	return ShipDistanceGroup;
}

void UShipObject::GetSpawnPositionAndRotation(FVector &OutVector, FRotator &OutRotation) const
{
	OutVector.X = (float)(PreciseShipLocation.X - UStaticUtilities::CurrentGlobalOffset().X);
	OutVector.Y = (float)(PreciseShipLocation.Y - UStaticUtilities::CurrentGlobalOffset().Y);
	OutVector.Z = (float)(PreciseShipLocation.Z - UStaticUtilities::CurrentGlobalOffset().Z);
	OutRotation = FRotator::ZeroRotator;
	//UE_LOG(LogTemp, Warning, TEXT("OutVector: %s"), *OutVector.ToString());
}

void UShipObject::UpdateFarLocation()
{
	LongShipLocation.X = (PreciseShipLocation.X / 1798754748);
	LongShipLocation.Y = (PreciseShipLocation.Y / 1798754748);
	LongShipLocation.Z = (PreciseShipLocation.Z / 1798754748);
	LongShipLocation /= 100.f;
}

FVector UShipObject::ConvertCloseToFarLocation(const FInt64Vector& InVector)
{
	FVector OutVector;
	OutVector.X = (InVector.X / 1798754748);
	OutVector.Y = (InVector.Y / 1798754748);
	OutVector.Z = (InVector.Z / 1798754748);
	OutVector /= 100.f;
	return OutVector;
}

FString UShipObject::GetFloatAsString(float Number, int Precision)
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
