// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Classes/Camera/CameraComponent.h"
#include "Classes/GameFramework/PlayerController.h"
#include "ExternalCamera.generated.h"

UCLASS()
class CUPCAKESINSPACE_API AExternalCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AExternalCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	USpringArmComponent* InnerSpringArm;
	UCameraComponent* SystemCamera;

	UPROPERTY(EditAnywhere)
	float MaxArmLength;
	UPROPERTY(EditAnywhere)
	float MinArmLength;
	UPROPERTY(EditAnywhere)
	float ZoomSpeed;

	UPROPERTY(EditAnywhere)
	APlayerController* PlayerController;
	FVector2D CameraInput;
	float ZoomFactor;
	float RestingArmLength;
	bool bRightClick;
	bool bLeftClick;

	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);
	void LeftClickDown();
	void LeftClickUp();
	void RightClickDown();
	void RightClickUp();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
