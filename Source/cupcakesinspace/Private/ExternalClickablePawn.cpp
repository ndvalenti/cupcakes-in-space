// Fill out your copyright notice in the Description page of Project Settings.

#include "ExternalClickablePawn.h"


// Sets default values for this component's properties
UExternalClickablePawn::UExternalClickablePawn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UExternalClickablePawn::SetParentAsClicked()
{
	PlayerController->SetCameraFocus(GetOwner());
}

// Called when the game starts
void UExternalClickablePawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AExternalPlayerController>(GetWorld()->GetFirstPlayerController());
}


// Called every frame
void UExternalClickablePawn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

