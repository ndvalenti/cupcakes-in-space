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
#include "Engine.h"

#include "ExternalCamera.generated.h"


class AMasterPlayerController;
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

UCLASS()
class CUPCAKESINSPACE_API AExternalCamera : public APawn
{
	GENERATED_BODY()

public:
	AExternalCamera();

	UPROPERTY(EditDefaultsOnly, Category = "Tick")
	FSecondTick SecondaryActorTick;

	virtual void RegisterActorTickFunctions(bool bRegister) override;
	virtual void PostInitProperties() override;
	void TickActor2 (float DeltaSeconds, ELevelTick TickType, FSecondTick &ThisTickFunction);
	void Tick2(float DeltaTime);

protected:
	virtual void BeginPlay() override;

	/*
	 * Child Components
	 */
	USpringArmComponent* InnerSpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* SystemCamera;

	/*
	 * Input and Camera Controls
	 */
	UPROPERTY(EditAnywhere, Category=Camera)
	float MaxArmLength;
	UPROPERTY(EditAnywhere, Category = Camera)
	float MinArmLength;
	UPROPERTY(EditAnywhere, Category = Camera)
	float ZoomSpeed;
	UPROPERTY(EditAnywhere, Category = Camera)
	float MaxOriginOffset;
	FVector2D CameraInput;
	float ZoomFactor;
	float RestingArmLength;
	bool bRightClick;
	bool bLeftClick;

	/*
	 * Origin Rebasing Declarations
	 */
	bool bMarkForRebasing;
	UPROPERTY(VisibleAnywhere)
	FIntVector LocalOriginOffset;
	AMasterPlayerController* MasterPlayerController;

	void RebaseOrigin();
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void LeftClickDown();
	void LeftClickUp();
	void RightClickDown();
	void RightClickUp();

	/*
	 * Debug Variables and Functions
	 */
	void DebugAction();
	bool bDebug;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
