// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

#include "SkyboxController.generated.h"


UCLASS()
class CUPCAKESINSPACE_API ASkyboxController : public AActor
{
	GENERATED_BODY()
	
public:	
	ASkyboxController();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	TLazyObjectPtr<AActor> CurrentCamera;

	virtual void Tick(float DeltaTime) override;
};
