// Fill out your copyright notice in the Description page of Project Settings.

#include "SystemCamera.h"


// Sets default values
ASystemCamera::ASystemCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RestingArmLength = 400.0f;
	MinArmLength = 200.0f;
	MaxArmLength = 1200.0f;
	ZoomSpeed = 300.0f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	InnerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	InnerSpringArm->SetupAttachment(RootComponent);
	//InnerSpringArm->SetRelativeLocationAndRotation(FVector(0.0F, 0.0F, 50.0F), FRotator(-60.0f, 0.0f, 0.0f));
	InnerSpringArm->TargetArmLength = RestingArmLength;
	InnerSpringArm->bEnableCameraRotationLag = true;
	InnerSpringArm->CameraLagSpeed = 3.0f;
	InnerSpringArm->bDoCollisionTest = false;

	SystemCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SystemCamera"));
	SystemCamera->SetupAttachment(InnerSpringArm, USpringArmComponent::SocketName);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	BaseEyeHeight = 0.0f;
}

// Called when the game starts or when spawned
void ASystemCamera::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASystemCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bRightClick)
	{
		if (bLeftClick)
		{
			ZoomFactor = DeltaTime * CameraInput.Y;
			ZoomFactor = ZoomSpeed * FMath::Clamp<float>(ZoomFactor, -1.0f, 1.0f);

			/*
			if (ZoomFactor > 0)
			{
				SystemCamera->FieldOfView = FMath::Lerp<float>(90.0f, 105.0f, ZoomFactor);
			}
			else
			{
				SystemCamera->FieldOfView = FMath::Lerp<float>(90.0f, 75.0f, -ZoomFactor);
			}
			*/
			InnerSpringArm->TargetArmLength = FMath::Clamp<float>(InnerSpringArm->TargetArmLength + ZoomFactor, MinArmLength, MaxArmLength);
		}
		else
		{
			FRotator NewYRotation = GetActorRotation();
			NewYRotation.Yaw += CameraInput.X;
			SetActorRotation(NewYRotation);

			FRotator NewPRotation = InnerSpringArm->GetComponentRotation();
			//NewPRotation.Pitch = FMath::Clamp(NewPRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
			NewPRotation.Pitch = NewPRotation.Pitch + CameraInput.Y;
			NewPRotation.Roll = 0.0f;
			InnerSpringArm->SetWorldRotation(NewPRotation);
		}
	}
}

// Called to bind functionality to input
void ASystemCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &ASystemCamera::LeftClickDown);
	PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &ASystemCamera::LeftClickUp);

	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &ASystemCamera::RightClickDown);
	PlayerInputComponent->BindAction("RightClick", IE_Released, this, &ASystemCamera::RightClickUp);

	PlayerInputComponent->BindAxis("MouseXAxis", this, &ASystemCamera::YawCamera);
	PlayerInputComponent->BindAxis("MouseYAxis", this, &ASystemCamera::PitchCamera);
}

void ASystemCamera::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void ASystemCamera::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void ASystemCamera::RightClickDown()
{

	bRightClick = true;
}

void ASystemCamera::RightClickUp()
{
	bRightClick = false;
}

void ASystemCamera::LeftClickDown()
{
	bLeftClick = true;
}

void ASystemCamera::LeftClickUp()
{
	bLeftClick = false;
}

