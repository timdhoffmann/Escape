// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	/// Variables

	UPhysicsHandleComponent* PhysicsHandleComponent = nullptr;
	UInputComponent* InputComponent = nullptr;
	
	float LineTraceReach = 100.f;
	
	/// Functions

	// Null checks and sets up assumed attached InputComponent.
	// Binds input events to actions.
	void SetupInputComponent();

	// Null checks and sets up assumed attached PhysicsHandleComponent.
	void SetupPhysicsHandleComponent();

	// Sets up and executes a LineTrace (RayCast) within reach.
	// Returns the object hit.
	FHitResult GetFirstPhysicsBodyInReach();

	// Moves a GrabbedComponent every tick, if one exists.
	void MoveGrabbedComponent();

	// Ray-casts and grabs resulting Actor in reach.
	void Grab();
	
	// Releases a grabbed Actor.
	void Release();

	// Returns LineTraceStart based on PlayerViewPoint.
	FVector GetLineTraceStart() const;

	// Returns LineTraceEnd based on PlayerViewPoint.
	FVector GetLineTraceEnd() const;
};
