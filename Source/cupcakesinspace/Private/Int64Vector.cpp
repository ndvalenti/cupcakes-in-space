#include "Int64Vector.h"
//#include "cmath"

const FInt64Vector FInt64Vector::ZeroVector(0, 0, 0);
const FInt64Vector FInt64Vector::OneVector(1, 1, 1);
const FInt64Vector FInt64Vector::UpVector(0, 0, 1);
const FInt64Vector FInt64Vector::ForwardVector(1, 0, 0);
const FInt64Vector FInt64Vector::RightVector(0, 1, 0);
const FInt64Vector FInt64Vector::NoneVector(INDEX_NONE, INDEX_NONE, INDEX_NONE);

FInt64Vector::FInt64Vector() :
	X(0),
	Y(0),
	Z(0)
{}

FInt64Vector::FInt64Vector(int32 InValue) :
	X(InValue),
	Y(InValue),
	Z(InValue)
{}

FInt64Vector::FInt64Vector(int64 InValue) :
	X(InValue),
	Y(InValue),
	Z(InValue)
{}

FInt64Vector::FInt64Vector(int64 InX, int64 InY, int64 InZ) :
	X(InX),
	Y(InY),
	Z(InZ)
{}

FInt64Vector::FInt64Vector(const FVector &InVector) :
	X(InVector.X),
	Y(InVector.Y),
	Z(InVector.Z)
{}

FInt64Vector::FInt64Vector(const FIntVector &InVector) :
	X(InVector.X),
	Y(InVector.Y),
	Z(InVector.Z)
{}

FInt64Vector::FInt64Vector(const FInt64Vector &InVector) :
	X(InVector.X),
	Y(InVector.Y),
	Z(InVector.Z)
{}

FInt64Vector::FInt64Vector(EForceInit) :
	X(0),
	Y(0),
	Z(0)
{}

bool FInt64Vector::operator==(const FInt64Vector& Other) const
{
	return X == Other.X && Y == Other.Y && Z == Other.Z;
}

bool FInt64Vector::operator!=(const FInt64Vector& Other) const
{
	return X != Other.X || Y != Other.Y || Z != Other.Z;
}

FInt64Vector& FInt64Vector::operator*=(int32 Scale)
{
	X *= X;
	Y *= Y;
	Z *= Z;

	return *this;
}

FInt64Vector& FInt64Vector::operator/=(int32 Divisor)
{
	X /= X;
	Y /= Y;
	Z /= Z;

	return *this;
}

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

FInt64Vector FInt64Vector::operator*(int32 Scale) const
{
	return FInt64Vector(*this) *= Scale;
}

FInt64Vector FInt64Vector::operator/(int32 Divisor) const
{
	return FInt64Vector(*this) /= Divisor;
}

FInt64Vector FInt64Vector::operator+(const FInt64Vector& Other) const
{
	return FInt64Vector(*this) += Other;
}

FInt64Vector FInt64Vector::operator-(const FInt64Vector& Other) const
{
	return FInt64Vector(*this) -= Other;
}

bool FInt64Vector::IsZero() const
{
	return *this == ZeroVector;
}

double FInt64Vector::Magnitude() const
{
	return sqrtl(SqLongLong(X) + SqLongLong(Y) + SqLongLong(Z));
}

FInt64Vector FInt64Vector::Normalize()
{
	return (*this / Magnitude());
}

int64 FInt64Vector::Distance(const FInt64Vector& From, const FInt64Vector& To)
{
	/*approximates distance between two points*/
	//return FMath::Sqrt(FMath::Square(To.X - From.X) + FMath::Square(To.Y - From.Y) + FMath::Square(To.Z - From.Z));
	/*
	double a[3] = { From.X, From.Y, From.Z };
	double b[3] = { To.X, To.Y, To.Z };
	double ans = FMath::Sqrt(FMath::Square(b[0] - a[0]) + FMath::Square(b[1] - a[1]) + FMath::Square(b[2] - a[2]));
	return (int64)ans;
	*/
	//return FMath::Sqrt(SqLongLong(To.X - From.X) + SqLongLong(To.Y - From.Y) + SqLongLong(To.Z - From.Z));
	//int64 debug = sqrtl(SqLongLong(To.X - From.X) + SqLongLong(To.Y - From.Y) + SqLongLong(To.Z - From.Z));
	//UE_LOG(LogTemp, Warning, TEXT("Dist: %lld"), debug);
	//return debug;
	return (int64)sqrtl(SqLongLong(To.X - From.X) + SqLongLong(To.Y - From.Y) + SqLongLong(To.Z - From.Z));
}

double FInt64Vector::SqLongLong(const int64& a)
{
	return (((double)a) * ((double)a));
}

FString FInt64Vector::ToString() const
{
	return FString::Printf(TEXT("X=%lld Y=%lld Z=%lld"), X, Y, Z);
}
