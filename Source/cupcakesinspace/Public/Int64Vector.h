// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Int64Vector.generated.h"

/**
 * A 3D vector composed of three int64 components (X, Y, Z)
 */

USTRUCT(BlueprintType)
struct FInt64Vector 
{
	GENERATED_BODY()

	/** Holds the x-coordinate */
	UPROPERTY(EditAnywhere)
	int64 X;

	/** Holds the y-coordinate */
	UPROPERTY(EditAnywhere)
	int64 Y;

	/** Holds the z-coordinate */
	UPROPERTY(EditAnywhere)
	int64 Z;

public:

	/** An int64 zero vector (0,0,0) */
	static const FInt64Vector ZeroVector;

	/** An int64 one vector (1,1,1) */
	static const FInt64Vector OneVector;

	/** An int64 up vector (0,0,1) */
	static const FInt64Vector UpVector;

	/** An int64 forward vector (1,0,0) */
	static const FInt64Vector ForwardVector;

	/** An int64 right vector (0,1,0) */
	static const FInt64Vector RightVector;

	/** An int64 empty vector (INDEX_NONE, INDEX_NONE, INDEX_NONE) */
	static const FInt64Vector NoneVector;

public:
	/** Default constructor (no initialization) */
	FInt64Vector();

	/**
	* Constructor initializing all components to a single value
	*
	* @param InValue Value to set all components to
	*/
	FInt64Vector(int32 InValue);

	/**
	* Constructor initializing all components to a single value
	*
	* @param InValue Value to set all components to
	*/
	FInt64Vector(int64 InValue);

	/**
	* Constructor using initial values for each component
	*
	* @param InX X Coordinate
	* @param InY Y Coordinate
	* @param InZ Z Coordinate
	*/
	FInt64Vector(int64 InX, int64 InY, int64 InZ);

	/**
	* Constructor using an existing FVector
	*
	* @param InVector
	*/
	FInt64Vector(const FVector &InVector);

	/**
	* Constructor using an existing FIntVector
	*
	* @param InVector
	*/
	FInt64Vector(const FIntVector &InVector);

	/**
	* Constructor using an existing FInt64Vector
	*
	* @param InVector
	*/
	FInt64Vector(const FInt64Vector &InVector);

	/**
	* Constructor to force initialization to 0
	*
	* @param EForceInit Force init enum
	*/
	FInt64Vector(EForceInit);

	/** Operator overloads */
	bool operator==(const FInt64Vector& Other) const;
	bool operator!=(const FInt64Vector& Other) const;
	FInt64Vector& operator*=(int32 Scale);
	FInt64Vector& operator/=(int32 Divisor);
	FInt64Vector& operator+=(const FInt64Vector& Add);
	FInt64Vector& operator-=(const FInt64Vector& Subtract);
	FInt64Vector& operator=(const FInt64Vector& Equals);
	FInt64Vector operator*(int32 Scale) const;
	FInt64Vector operator/(int32 Divisor) const;
	FInt64Vector operator+(const FInt64Vector& Other) const;
	FInt64Vector operator-(const FInt64Vector& Other) const;

	bool IsZero() const;
	double Magnitude() const;
	FInt64Vector Normalize();
	static int64 Distance(const FInt64Vector& From, const FInt64Vector& To);

	static double SqLongLong(const int64& a);

	FString ToString() const;
	
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
};