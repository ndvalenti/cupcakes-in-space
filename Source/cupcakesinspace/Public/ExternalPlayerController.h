// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExternalPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CUPCAKESINSPACE_API AExternalPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Returns a pointer to the pawn the player is currently possessing
	//AExternalCamera* GetExternalCamera() const;
	UFUNCTION(BlueprintCallable)
	void SetCameraFocus(AActor *FocusedPawn);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	TLazyObjectPtr<AActor> CameraFocus;
	
};
