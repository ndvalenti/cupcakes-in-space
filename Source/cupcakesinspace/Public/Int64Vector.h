// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Int64Vector.generated.h"

/**
 * A limited implementation of FIntVector using int64 elements
 */

USTRUCT()
struct FInt64Vector {
	GENERATED_BODY()
	int64 X;
	int64 Y;
	int64 Z;

public:
	FInt64Vector();
	FInt64Vector(int64 x, int64 y, int64 z);
	FInt64Vector(const FInt64Vector &InVector);

	/*Operator overloads*/
	FInt64Vector& operator+=(const FInt64Vector& Add);
	FInt64Vector& operator-=(const FInt64Vector& Subtract);
	FInt64Vector& operator=(const FInt64Vector& Equals);

	/*FIntVector operator overloads*/
	FInt64Vector& operator+=(const FIntVector& Add);
	FInt64Vector& operator-=(const FIntVector& Subtract);
	
	const FString ToString();
	const static int64 Distance(const FInt64Vector& From, const FInt64Vector& To);
	const static int IntDistance(const FInt64Vector& From, const FInt64Vector& To);
	
	//const static FORCEINLINE FInt64Vector& FInt64Vector::ZeroValue() { return FInt64Vector::FInt64Vector(0, 0, 0); }
	/**
	* Serializes the Rectangle.
	*
	* @param Ar The archive to serialize into.
	* @param Vector The vector to serialize.
	* @return Reference to the Archive after serialization.
	* 
	* This needs to be tested and serialization needs to be researched
	* currently this is only the implementation of IntVector.h
	*/
	
	friend FArchive& operator<<(FArchive& Ar, FInt64Vector& Vector)
	{
		return Ar << Vector.X << Vector.Y << Vector.Z;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << *this;
		return true;
	}
	
protected:
	static FInt64Vector _ZeroVector;

public:
	const static FInt64Vector ZeroVector();
};