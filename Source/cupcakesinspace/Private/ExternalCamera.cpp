// Fill out your copyright notice in the Description page of Project Settings.
#include "ExternalCamera.h"
#include "MasterPlayerController.h"

AExternalCamera::AExternalCamera()
{
	/* 
	 * Tick Group Initializations
	 */
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SecondaryActorTick.TickGroup = TG_PostPhysics;
	SecondaryActorTick.bCanEverTick = true;
	SecondaryActorTick.bStartWithTickEnabled = true;

	/* 
	 * Camera Settings and Behavious
	 */
	RestingArmLength = 1000.0f;
	MinArmLength = 800.0f;
	MaxArmLength = 350000.0f;
	ZoomSpeed = 150000.0f;
	MaxOriginOffset = 50000.f;
	LocalOriginOffset = LocalOriginOffset.ZeroValue;

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

void AExternalCamera::BeginPlay()
{
	Super::BeginPlay();

	bDebug = false;
	bMarkForRebasing = false;
	MasterPlayerController = Cast<AMasterPlayerController>(GetWorld()->GetFirstPlayerController());
}

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
			GEngine->AddOnScreenDebugMessage(1, .5f, FColor::Cyan, FString::Printf(TEXT("Rebasing Origin to %s, Dist: %f"), *SystemCamera->GetComponentLocation().ToString()), DistFromOrigin);
		}
		bMarkForRebasing = true;
	}
}

void AExternalCamera::Tick2(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(2, .5f, FColor::Green, FString::Printf(TEXT("Location: %s, Dist: %f"), *SystemCamera->GetComponentLocation().ToString(), SystemCamera->GetComponentLocation().Size()));

	FIntVector ShiftedOrigin = FIntVector(SystemCamera->GetComponentLocation());
	GEngine->AddOnScreenDebugMessage(3, .5f, FColor::Red, FString::Printf(TEXT("Location: %s"), *ShiftedOrigin.ToString()));
	
	if (bMarkForRebasing)
	{
		RebaseOrigin();
		bMarkForRebasing = false;
	}
}

void AExternalCamera::DebugAction()
{
	if (MasterPlayerController)
	{
		MasterPlayerController->SpawnAllAtLocation();
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Something terrible has happened: playercontroller UA"))
	}
}

void AExternalCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AExternalCamera::LeftClickDown);
	PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &AExternalCamera::LeftClickUp);

	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &AExternalCamera::RightClickDown);
	PlayerInputComponent->BindAction("RightClick", IE_Released, this, &AExternalCamera::RightClickUp);

	PlayerInputComponent->BindAxis("MouseXAxis", this, &AExternalCamera::YawCamera);
	PlayerInputComponent->BindAxis("MouseYAxis", this, &AExternalCamera::PitchCamera);

	//PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AExternalCamera::DebugAction);
}

void AExternalCamera::RebaseOrigin()
{
	FIntVector ShiftedOrigin = FIntVector(SystemCamera->GetComponentLocation());
	LocalOriginOffset += ShiftedOrigin;
	UStaticUtilities::TranslateGlobalOffset(ShiftedOrigin);
	GetWorld()->RequestNewWorldOrigin(LocalOriginOffset);
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

void FSecondTick::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef & MyCompletionGraphEvent)
{
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
