// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalPawn.h"
#include "GameStateManager.h"

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

	Manager = Cast<AGameStateManager>(GetWorld()->GetGameState());

	if (bClickable) {
		OnClicked.AddUniqueDynamic(this, &ALocalPawn::PawnClicked);
	}
}

// Called every frame
void ALocalPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//AActor* ALocalPawn::PawnClicked(AActor* TouchedActor, FKey ButtonPressed)
// The more I look at this the more horrified it makes me
// TODO: literally anything else, can depreciate to an actor with a line trace in MasterPlayerController
void ALocalPawn::PawnClicked(AActor* TouchedActor, FKey ButtonPressed)
{
	Cast<AMasterPlayerController>(GetWorld()->GetFirstPlayerController())->SetCameraFocus(this);
}

