// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "UObjectBaseUtility.h"
#include "ObjectMacros.h"

#include "IntVector.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "ExternalCamera.generated.h"


// START TEST
USTRUCT()
struct FSecondTick : public FTickFunction
{
	GENERATED_USTRUCT_BODY()

	class AExternalCamera* Target;

	CUPCAKESINSPACE_API virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef &MyCompletionGraphEvent) override;

	CUPCAKESINSPACE_API virtual FString DiagnosticMessage() override;

};

template<>
struct TStructOpsTypeTraits<FSecondTick> : public TStructOpsTypeTraitsBase2<FSecondTick>
{
	enum { WithCopy = false };
};
//END TEST

UCLASS()
class CUPCAKESINSPACE_API AExternalCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AExternalCamera();

	// START TEST
	UPROPERTY(EditDefaultsOnly, Category = "Tick")
	FSecondTick SecondaryActorTick;

	virtual void RegisterActorTickFunctions(bool bRegister) override;
	virtual void PostInitProperties() override;
	void TickActor2 (float DeltaSeconds, ELevelTick TickType, FSecondTick &ThisTickFunction);
	void Tick2(float DeltaTime);
	// END TEST

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	USpringArmComponent* InnerSpringArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent* SystemCamera;

	UPROPERTY(EditAnywhere)
	float MaxArmLength;
	UPROPERTY(EditAnywhere)
	float MinArmLength;
	UPROPERTY(EditAnywhere)
	float ZoomSpeed;
	UPROPERTY(EditAnywhere)
	float MaxOriginOffset;
	//UPROPERTY(EditAnywhere)
	//TArray<AActor*> LocalObjects;
	//UPROPERTY(EditAnywhere)
	//TArray<ASkyboxController*> Skybox;
	//UPROPERTY(EditAnywhere)
	//TArray<ACelestialController*> Celestials;
	
	

	FVector2D CameraInput;
	float ZoomFactor;
	float RestingArmLength;
	bool bRightClick;
	bool bLeftClick;

	bool bRebase;
	UPROPERTY(EditAnywhere)
	FIntVector CurrentOffset;
	void RebaseOrigin();
	void PopulateActors();

	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void LeftClickDown();
	void LeftClickUp();
	void RightClickDown();
	void RightClickUp();

	bool bDebug;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
	
};
