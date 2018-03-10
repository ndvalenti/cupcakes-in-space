// Fill out your copyright notice in the Description page of Project Settings.

/*
 * In order to add a custom class, add the proper parent and generated include files
 * and add UCLASS() and GENERATED_BODY(), after delete the 
 * .vs, Binaries, Intermediate, and Saved folders and *.sln file and
 * regenerate project files, ensure class prefixes are correct
 */

#pragma once

#include "CoreMinimal.h"
#include "Object.h"
#include "ShipObject.generated.h"

UCLASS()
class CUPCAKESINSPACE_API UShipObject : public UObject
{
	GENERATED_BODY()

public:
	UShipObject();

};
