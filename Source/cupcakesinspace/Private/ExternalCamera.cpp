// Fill out your copyright notice in the Description page of Project Settings.

#include "ExternalCamera.h"


// Sets default values
AExternalCamera::AExternalCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RestingArmLength = 1000.0f;
	UPROPERTY(EditAnywhere)
	MinArmLength = 800.0f;
	UPROPERTY(EditAnywhere)
	MaxArmLength = 150000.0f;
	UPROPERTY(EditAnywhere)
	ZoomSpeed = 50000.0f;

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
void AExternalCamera::BeginPlay()
{
	Super::BeginPlay();
	MaxOriginOffset = 50000.f;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALocalPawn::StaticClass(), LocalObjects);
	bDebug = false;
}

// Called every frame
void AExternalCamera::Tick(float DeltaTime)
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
	
	float DistFromOrigin = FVector::Dist(SystemCamera->GetComponentLocation(), FVector::ZeroVector);

	if (DistFromOrigin > MaxOriginOffset) {
		if (!bDebug) {
			UE_LOG(LogTemp, Warning, TEXT("Rebasing Origin to %s, Dist: %f"), *SystemCamera->GetComponentLocation().ToString(), DistFromOrigin);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Rebasing Origin to %s, Dist: %f"), *SystemCamera->GetComponentLocation().ToString(), DistFromOrigin));
			RebaseOrigin();
			//bDebug = true;
		}
	}
}

// Called to bind functionality to input
void AExternalCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AExternalCamera::LeftClickDown);
	PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &AExternalCamera::LeftClickUp);

	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &AExternalCamera::RightClickDown);
	PlayerInputComponent->BindAction("RightClick", IE_Released, this, &AExternalCamera::RightClickUp);

	PlayerInputComponent->BindAxis("MouseXAxis", this, &AExternalCamera::YawCamera);
	PlayerInputComponent->BindAxis("MouseYAxis", this, &AExternalCamera::PitchCamera);
}

void AExternalCamera::RebaseOrigin()
{
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALocalPawn::StaticClass(), LocalObjects);

	//DrawDebugSphere(GetWorld(), SystemCamera->GetComponentLocation(), 100.f, 32, FColor::Green, true);
	//DrawDebugLine(GetWorld(), FVector::ZeroVector, SystemCamera->GetComponentLocation(), FColor::Green, true);
	FVector ShiftedOrigin = SystemCamera->GetComponentLocation();

	for (auto& LP : LocalObjects) {
		//UE_LOG(LogTemp, Warning, TEXT("Shifting %s from %s"), *LP->GetName(), *LP->GetActorLocation().ToString());
		LP->SetActorLocation(LP->GetActorLocation() - ShiftedOrigin);
		//UE_LOG(LogTemp, Warning, TEXT("to %s"), *LP->GetActorLocation().ToString());
	}
	//UE_LOG(LogTemp, Warning, TEXT("Shifting Origin from %s"), *GetActorLocation().ToString());
	SetActorLocation(GetActorLocation() - ShiftedOrigin);
	//UE_LOG(LogTemp, Warning, TEXT("to %s"), *GetActorLocation().ToString());
}

void AExternalCamera::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AExternalCamera::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AExternalCamera::RightClickDown()
{

	bRightClick = true;
}

void AExternalCamera::RightClickUp()
{
	bRightClick = false;
}

void AExternalCamera::LeftClickDown()
{
	bLeftClick = true;
}

void AExternalCamera::LeftClickUp()
{
	bLeftClick = false;
}

