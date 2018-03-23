#include "Int64Vector.h"

FInt64Vector FInt64Vector::_ZeroVector = FInt64Vector::FInt64Vector(0, 0, 0);

FInt64Vector::FInt64Vector() :
	X(0),
	Y(0),
	Z(0)
{}

FInt64Vector::FInt64Vector(int64 x, int64 y, int64 z) :
	X(x),
	Y(y),
	Z(z)
{}

FInt64Vector::FInt64Vector(const FInt64Vector &InVector) :
	X(InVector.X),
	Y(InVector.Y),
	Z(InVector.Z)
{}

FInt64Vector& FInt64Vector::operator+=(const FInt64Vector& Add)
{
	X += Add.X;
	Y += Add.Y;
	Z += Add.Z;

	return *this;
}

FInt64Vector& FInt64Vector::operator-=(const FInt64Vector& Subtract)
{
	X -= Subtract.X;
	Y -= Subtract.Y;
	Z -= Subtract.Z;

	return *this;
}

FInt64Vector& FInt64Vector::operator=(const FInt64Vector& Equals)
{
	X = Equals.X;
	Y = Equals.Y;
	Z = Equals.Z;

	return *this;
}

FInt64Vector& FInt64Vector::operator+=(const FIntVector& Add)
{
	X += Add.X;
	Y += Add.Y;
	Z += Add.Z;

	return *this;
}

FInt64Vector& FInt64Vector::operator-=(const FIntVector& Subtract)
{
	X -= Subtract.X;
	Y -= Subtract.Y;
	Z -= Subtract.Z;

	return *this;
}

const int64 FInt64Vector::Distance(const FInt64Vector& From, const FInt64Vector& To)
{
	return FMath::Sqrt(FMath::Square(To.X - From.X) + FMath::Square(To.Y - From.Y) + FMath::Square(To.Z - From.Z));
}

const int FInt64Vector::IntDistance(const FInt64Vector& From, const FInt64Vector& To)
{
	return FMath::Sqrt(FMath::Square(To.X - From.X) + FMath::Square(To.Y - From.Y) + FMath::Square(To.Z - From.Z));
}

const FString FInt64Vector::ToString()
{
	return FString::Printf(TEXT("X=%d Y=%d Z=%d"), X, Y, Z);
}

const FORCEINLINE FInt64Vector FInt64Vector::ZeroVector()
{
	return _ZeroVector;
}