// Fill out your copyright notice in the Description page of Project Settings.

#include "ExternalCamera.h"



// Sets default values
AExternalCamera::AExternalCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	// BEGIN TEST (Uncomment primary actor tick above if deleting)
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SecondaryActorTick.TickGroup = TG_PostPhysics;
	SecondaryActorTick.bCanEverTick = true;
	SecondaryActorTick.bStartWithTickEnabled = true;
	// END TEST

	RestingArmLength = 1000.0f;
	MinArmLength = 800.0f;
	MaxArmLength = 350000.0f;
	ZoomSpeed = 150000.0f;
	CurrentOffset = CurrentOffset.ZeroValue;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	InnerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	InnerSpringArm->SetupAttachment(RootComponent);
	//InnerSpringArm->SetRelativeLocationAndRotation(FVector(0.0F, 0.0F, 50.0F), FRotator(-60.0f, 0.0f, 0.0f));
	InnerSpringArm->TargetArmLength = RestingArmLength;

	//InnerSpringArm->CameraLagMaxDistance = 10000.f;
	//InnerSpringArm->bEnableCameraLag = true;

	InnerSpringArm->bDrawDebugLagMarkers = true;
	InnerSpringArm->bEnableCameraRotationLag = true;
	InnerSpringArm->CameraLagSpeed = 3.0f;

	InnerSpringArm->bDoCollisionTest = false;

	SystemCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SystemCamera"));
	SystemCamera->SetupAttachment(InnerSpringArm, USpringArmComponent::SocketName);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	BaseEyeHeight = 0.0f;
}

// BEGIN TEST

void AExternalCamera::TickActor2(float DeltaSeconds, ELevelTick TickType, FSecondTick & ThisTickFunction)
{
	const bool bShouldTick = ((TickType != LEVELTICK_ViewportsOnly) || ShouldTickIfViewportsOnly());
	if (bShouldTick)
	{
		if (!IsPendingKill() && GetWorld())
		{
			Tick2(DeltaSeconds);
		}
	}
}

void AExternalCamera::RegisterActorTickFunctions(bool bRegister)
{
	if (bRegister)
	{
		if (SecondaryActorTick.bCanEverTick)
		{
			SecondaryActorTick.Target = this;
			SecondaryActorTick.SetTickFunctionEnable(PrimaryActorTick.bStartWithTickEnabled || PrimaryActorTick.IsTickFunctionEnabled());
			SecondaryActorTick.RegisterTickFunction(GetLevel());
		}
	}
	else
	{
		if (SecondaryActorTick.IsTickFunctionRegistered())
		{
			SecondaryActorTick.UnRegisterTickFunction();
		}
	}
	Super::RegisterActorTickFunctions(bRegister);
}

void AExternalCamera::PostInitProperties()
{
	Super::PostInitProperties();
	if (!IsTemplate() && SecondaryActorTick.bCanEverTick)
	{
		SecondaryActorTick.Target = this;
		SecondaryActorTick.SetTickFunctionEnable(SecondaryActorTick.bStartWithTickEnabled);
		SecondaryActorTick.RegisterTickFunction(GetLevel());
	}
}

// END TEST


// Called when the game starts or when spawned
void AExternalCamera::BeginPlay()
{
	Super::BeginPlay();
	MaxOriginOffset = 50000.f;

	PopulateActors();

	bDebug = false;
	bRebase = false;
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
	
	float DistFromOrigin = SystemCamera->GetComponentLocation().Size();

	if (DistFromOrigin > MaxOriginOffset) {
		if (!bDebug) {
			UE_LOG(LogTemp, Warning, TEXT("Rebasing Origin to %s, Dist: %f"), *SystemCamera->GetComponentLocation().ToString(), DistFromOrigin);
			GEngine->AddOnScreenDebugMessage(1, .5f, FColor::Cyan, FString::Printf(TEXT("Rebasing Origin to %s, Dist: %f"), *SystemCamera->GetComponentLocation().ToString(), DistFromOrigin));
		}
		bRebase = true;
	}
}

void AExternalCamera::Tick2(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(2, .5f, FColor::Green, FString::Printf(TEXT("Location: %s, Dist: %f"), *SystemCamera->GetComponentLocation().ToString(), SystemCamera->GetComponentLocation().Size()));

	FIntVector ShiftedOrigin = FIntVector(SystemCamera->GetComponentLocation());
	GEngine->AddOnScreenDebugMessage(3, .5f, FColor::Red, FString::Printf(TEXT("Location: %s"), *ShiftedOrigin.ToString()));
	
	if (bRebase)
	{
		RebaseOrigin();
		bRebase = false;
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

	//PlayerInputComponent->BindAxis("Action", this, );
}

void AExternalCamera::RebaseOrigin()
{
	FIntVector ShiftedOrigin = FIntVector(SystemCamera->GetComponentLocation());
	CurrentOffset += ShiftedOrigin;
	GetWorld()->RequestNewWorldOrigin(CurrentOffset);

	/*
	for (auto& LP : LocalObjects) {
		LP->SetActorLocation(LP->GetActorLocation() - ShiftedOrigin);
	}

	for (auto& LP : Skybox) {
		LP->SetActorLocation(LP->GetActorLocation() - ShiftedOrigin);
	}

	for (auto& LP : Celestials) {
		LP->SetActorLocation(LP->GetActorLocation() - ShiftedOrigin);
	}
	
	SetActorLocation(GetActorLocation() - ShiftedOrigin);
	*/
}

void AExternalCamera::PopulateActors()
{
	/*
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALocalPawn::StaticClass(), LocalObjects);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASkyboxController::StaticClass(), Skybox);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialController::StaticClass(), Celestials);

	for (TActorIterator<ASkyboxController> It(GetWorld()); It; ++It)
	{
		ASkyboxController* Actor = *It;
		if (!Actor->IsPendingKill())
		{
			Skybox.Add(Actor);
			//Actor->AddTickPrerequisiteComponent(InnerSpringArm);
		}
	}
	for (TActorIterator<ACelestialController> It(GetWorld()); It; ++It)
	{
		ACelestialController* Actor = *It;
		if (!Actor->IsPendingKill())
		{
			Celestials.Add(Actor);
			//Actor->AddTickPrerequisiteComponent(InnerSpringArm);
		}
	}
	*/
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

// BEGIN TEST
void FSecondTick::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef & MyCompletionGraphEvent)
{
	//if (Target && !Target->HasAnyFlags(RF_PendingKill | RF_Unreachable))
	if (Target && !Target->IsPendingKillOrUnreachable())
	{
		FScopeCycleCounterUObject ActorScope(Target);
		Target->TickActor2(DeltaTime*Target->CustomTimeDilation, TickType, *this);
	}
}

FString FSecondTick::DiagnosticMessage()
{
	return Target->GetFullName() + TEXT("[TickActor2]");
}
// END TEST
