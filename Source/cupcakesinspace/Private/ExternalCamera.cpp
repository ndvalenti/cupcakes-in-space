// Fill out your copyright notice in the Description page of Project Settings.
#include "ExternalCamera.h"
#include "ShipObject.h"
#include "LocalSpawnableObject.h"
#include "MasterPlayerController.h"

#include "Engine/StaticMesh.h"

/*-------------------PUBLIC MEMBERS-------------------*/
AExternalCamera::AExternalCamera() : RestingArmLength(1000.f), MinArmLength(800.f), MaxArmLength(350000.f), ZoomSpeed(150000.f)
{
	/*Primary and Secondary Tick Group Initializations*/
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SecondaryActorTick.TickGroup = TG_PostPhysics;
	SecondaryActorTick.bCanEverTick = true;
	SecondaryActorTick.bStartWithTickEnabled = true;

	/*Create camera components*/
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	InnerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SystemCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SystemCamera"));

	/*Camera and Spring Arm Initializations*/
	InnerSpringArm->SetupAttachment(RootComponent);
	InnerSpringArm->TargetArmLength = RestingArmLength;
	InnerSpringArm->bEnableCameraRotationLag = true;
	InnerSpringArm->CameraLagSpeed = 3.0f;
	InnerSpringArm->bDoCollisionTest = false;
	//InnerSpringArm->SetRelativeLocationAndRotation(FVector(0.0F, 0.0F, 50.0F), FRotator(-60.0f, 0.0f, 0.0f));

	LocalOriginOffset = LocalOriginOffset.ZeroValue;
	SystemCamera->SetupAttachment(InnerSpringArm, USpringArmComponent::SocketName);
	BaseEyeHeight = 0.0f;

	/*Auto Possess this pawn until logic is written for internal and external views*/
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AExternalCamera::RebaseOrigin()
{
	FIntVector ShiftedOrigin = FIntVector(SystemCamera->GetComponentLocation());
	LocalOriginOffset += ShiftedOrigin;
	UStaticUtilities::TranslateGlobalOffset(ShiftedOrigin);
	GetWorld()->RequestNewWorldOrigin(LocalOriginOffset);
}

/*-------------------PRIVATE MEMBERS-------------------*/
void AExternalCamera::BeginPlay()
{
	Super::BeginPlay();

	//bDebug = false;
	bMarkForRebasing = false;
	MasterPlayerController = Cast<AMasterPlayerController>(GetWorld()->GetFirstPlayerController());
	//GetWorld()->GetGameState<AGameStateManager>()->SetPossessedObject(this);
}

void AExternalCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraFocus();
	SetActorLocation(CameraFocalVector);

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

	if (DistFromOrigin > UStaticUtilities::RebaseDistance())
	{
		/*
		if (!bDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("Rebasing Origin to %s, Dist: %f"), *SystemCamera->GetComponentLocation().ToString(), DistFromOrigin);
		}
		*/
		bMarkForRebasing = true;
	}
}

void AExternalCamera::UpdateCameraFocus()
{
	if (CameraFocalPoints.Num() > 0)
	{
		CameraFocalVector = CameraFocalPoints[0]->GetActorLocation();
	}
	else
	{
		CameraFocalVector = FVector::FVector(0.f, 0.f, 0.f);
	}
}

void AExternalCamera::SetNewCameraFocus(AActor* In)
{
	CameraFocalPoints.Empty();
	CameraFocalPoints.Add(In);
}

void AExternalCamera::SetNewCameraFocus(TArray<AActor*> In)
{
	//CameraFocalPoints.Empty();
	//for (auto& Actor : In)
	//{
	//	CameraFocalPoints.Add(In);
	//}
}

void AExternalCamera::SetCameraSystemLocation(FInt64Vector& In)
{
	CameraSystemLocation = In;
}

void AExternalCamera::Tick2(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FIntVector ShiftedOrigin = FIntVector(SystemCamera->GetComponentLocation());

	if (bMarkForRebasing)
	{
		RebaseOrigin();
		bMarkForRebasing = false;
	}
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

void AExternalCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AExternalCamera::LeftClickDown);
	PlayerInputComponent->BindAction("LeftClick", IE_Released, this, &AExternalCamera::LeftClickUp);

	PlayerInputComponent->BindAction("RightClick", IE_Pressed, this, &AExternalCamera::RightClickDown);
	PlayerInputComponent->BindAction("RightClick", IE_Released, this, &AExternalCamera::RightClickUp);

	PlayerInputComponent->BindAxis("MouseXAxis", this, &AExternalCamera::YawCamera);
	PlayerInputComponent->BindAxis("MouseYAxis", this, &AExternalCamera::PitchCamera);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AExternalCamera::DebugAction);
}

void AExternalCamera::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AExternalCamera::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AExternalCamera::LeftClickDown()
{
	bLeftClick = true;
}

void AExternalCamera::LeftClickUp()
{
	bLeftClick = false;
}

void AExternalCamera::RightClickDown()
{
	bRightClick = true;
}

void AExternalCamera::RightClickUp()
{
	bRightClick = false;
}

void AExternalCamera::DebugAction()
{
	AGameStateManager* GameStateManager = GetWorld()->GetGameState<AGameStateManager>();
	if (GameStateManager)
	{
		//GameStateManager->SetAsPlayerShip(6);
		//for (auto& Local : GameStateManager->SpawnedShips)
		//{
		//	if (Local->GetShipIndex() == 6)
		//	{
		//		GameStateManager->DespawnShip(Local);
		//		break;
		//	}
		//}
		//GameStateManager->DespawnShip(GameStateManager->SpawnedPlayerShip);
		//FInt64Vector PlayerTarget(FInt64Vector::FInt64Vector(141569252166, -1078668714713, -23049890318) - FInt64Vector::FInt64Vector(2400000000, -1500000000, -500000000));

		//GameStateManager->SpawnedPlayerShip->WarpTo(PlayerTarget);
		for (auto& Local : GameStateManager->SpawnedShips)
		{
			if (Local->GetShipIndex() == 4)
			{
				TArray<UStaticMeshComponent*> Components;
				Local->GetComponents<UStaticMeshComponent>(Components);
				UStaticMeshComponent* StaticMeshComponent = Components[0];
				UStaticMesh* StaticMesh = StaticMeshComponent->GetStaticMesh();

				StaticMeshComponent->AddForce(FVector::ForwardVector * 10000, NAME_None, true);

				//SetNewCameraFocus(Local);
				break;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Something terrible has happened: GameStateManager UA"))
	}
	
}

/*-------------------FSecondTick Functions-------------------*/
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
