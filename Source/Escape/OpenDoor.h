// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

// Needed for UPROPERTY(BlueprintAssignable) to work.
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	/// Custom declarations.

	/// Needs DECLARE_DYNAMIC_MULTICAST_DELEGATE in order to work(see above).
	// Triggers door opening animation in blueprint.
	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpenRequest;

	/// Needs DECLARE_DYNAMIC_MULTICAST_DELEGATE in order to work(see above).
	// Triggers door closing animation in blueprint.
	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnCloseRequest;

private:
	UPROPERTY(EditAnywhere) 
		ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
		float TriggerMassThreshold = 50.f;

	/// Functions
	// Gets the total mass of actors on the pressure plate.
	float GetTotalMassOfActorsOnPlate();
};
