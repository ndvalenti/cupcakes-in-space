// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "UObjectBaseUtility.h"
#include "ObjectMacros.h"

#include "Int64Vector.h"
#include "IntVector.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

#include "ExternalCamera.generated.h"

class ALocalSpawnableObject;
class UShipObject;
class AMasterPlayerController;

USTRUCT()
struct FSecondTick : public FTickFunction
{
	GENERATED_BODY()

	class AExternalCamera* Target;

	CUPCAKESINSPACE_API virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef &MyCompletionGraphEvent) override;

	CUPCAKESINSPACE_API virtual FString DiagnosticMessage() override;

};

template<>
struct TStructOpsTypeTraits<FSecondTick> : public TStructOpsTypeTraitsBase2<FSecondTick>
{
	enum { WithCopy = false };
};

UCLASS()
class CUPCAKESINSPACE_API AExternalCamera : public APawn
{
	GENERATED_BODY()
	friend FSecondTick;

public:
	AExternalCamera();

	/**
	* Forces a rebase of the origin to the current camera position at the next PostPhysics tick
	*/
	void RebaseOrigin();

	/**
	*
	*/
	void UpdateCameraFocus();

	/**
	*
	*/
	void SetNewCameraFocus(AActor* In);

	/**
	*
	*/
	void SetNewCameraFocus(TArray<AActor*> In);

	/**
	*
	*/
	void SetCameraSystemLocation(FInt64Vector& In);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/*
	* Standard tick functions to create and use this object's late tick
	* Tick2() is the function to be called every tick
	*/
	void Tick2(float DeltaTime);
	void TickActor2(float DeltaSeconds, ELevelTick TickType, FSecondTick &ThisTickFunction);
	virtual void RegisterActorTickFunctions(bool bRegister) override;
	virtual void PostInitProperties() override;

	/*Set up controls while in an external mode*/
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/*Functions to be called when inputs are recognized*/
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void LeftClickDown();
	void LeftClickUp();
	void RightClickDown();
	void RightClickUp();

protected:
	/*UPROPERTY variables*/
	/**The Camera's precise location in the system*/
	UPROPERTY()
	FInt64Vector CameraSystemLocation;

	/**The Camera's current focal point in the world*/
	UPROPERTY(VisibleAnywhere)
	FVector CameraFocalVector;

	UPROPERTY()
	TArray<AActor*> CameraFocalPoints;

	/**Pointer to current game camera*/
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* SystemCamera;

	/**Current local origin offset*/
	UPROPERTY(VisibleAnywhere)
	FIntVector LocalOriginOffset;

	/**Second tick requirement*/
	UPROPERTY(EditDefaultsOnly, Category = "Tick")
	FSecondTick SecondaryActorTick;

	/**Camera Max Zoom*/
	UPROPERTY(VisibleAnywhere, Category = Camera)
	float MaxArmLength;

	/**Camera Min Zoom*/
	UPROPERTY(VisibleAnywhere, Category = Camera)
	float MinArmLength;

	/**Camera movement speed*/
	UPROPERTY(VisibleAnywhere, Category = Camera)
	float ZoomSpeed;

	/**Persistant pointer to the master player controller*/
	UPROPERTY()
	AMasterPlayerController* MasterPlayerController;

	/**Persistant pointer to the created spring arm component*/
	UPROPERTY()
	USpringArmComponent* InnerSpringArm;
	
	/**When set to true the late tick will initiate an origin rebase*/
	bool bMarkForRebasing;

	/*
	* Additional Input and Camera Controls
	*/
	FVector2D CameraInput;
	float ZoomFactor;
	float RestingArmLength;
	bool bRightClick;
	bool bLeftClick;

	/**
	* Debug Variables and Functions
	*/
	void DebugAction();
};
