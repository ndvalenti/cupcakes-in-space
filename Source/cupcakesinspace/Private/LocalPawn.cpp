// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalPawn.h"


// Sets default values
ALocalPawn::ALocalPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//OnClicked.AddUniqueDynamic(this, &ALocalPawn::PawnClicked);
}

// Called when the game starts or when spawned
void ALocalPawn::BeginPlay()
{
	Super::BeginPlay();
	if (bClickable) {
		OnClicked.AddUniqueDynamic(this, &ALocalPawn::PawnClicked);
	}
}

// Called every frame
void ALocalPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALocalPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALocalPawn::PawnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	Cast<AExternalPlayerController>(GetWorld()->GetFirstPlayerController())->SetCameraFocus(this);
}

