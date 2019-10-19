// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "Grabber.h"
//#include "DebugViewModeHelpers.h" // Might be needed for IWYU.

// Points out variable modification by certain functions when passed in as argument.
#define OUT // A macro that compiles to nothing. 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	SetupPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	MoveGrabbedComponent();
}

void UGrabber::MoveGrabbedComponent()
{
	if (!PhysicsHandleComponent) { return; }
	if (PhysicsHandleComponent->GrabbedComponent)
	{
		// Move the object that we're holding with offset.
		PhysicsHandleComponent->SetTargetLocation(GetLineTraceEnd());
	}
}

// InputComponent only appears at run time.
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if 
		(
			ensureMsgf(InputComponent != nullptr,
			TEXT("%s: InputComponent not found. Should be attached at run time."), *GetOwner()->GetName())
		)
	{
		// Bind input events.
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::SetupPhysicsHandleComponent()
{
	PhysicsHandleComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandleComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s (%s): PhysicsHandleComponent not found."), *GetOwner()->GetName(), *this->GetName());
	}
}

void UGrabber::Grab()
{
	// Line trace for first actor in reach with physics body collision channel set.
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case.
	auto ActorHit = HitResult.GetActor();

	// Grab ComponentToGrab.
	if (ActorHit)
	{
		if (!PhysicsHandleComponent) { return; }
		PhysicsHandleComponent->GrabComponentAtLocationWithRotation
		(
			ComponentToGrab, 
			NAME_None, // no bones needed.
			ComponentToGrab->GetOwner()->GetActorLocation(), 
			ComponentToGrab->GetOwner()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandleComponent) { return; }
	if (PhysicsHandleComponent->GrabbedComponent)
	{
		PhysicsHandleComponent->ReleaseComponent();
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Set up parameters used for the LineTrace for clarity.
	FHitResult HitResult;

	FCollisionObjectQueryParams ObjectTypesLookedFor(ECollisionChannel::ECC_PhysicsBody);

	FCollisionQueryParams AdditionalTraceParameters
	(
		FName(TEXT("")),	// use empty tag,
		false,				// use simplified collider,
		GetOwner()			// ignore ourselves (because the LineTrace starts in the center of our body).
	);

	// Execute LineTrace (Ray-cast).
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetLineTraceStart(),
		GetLineTraceEnd(),
		ObjectTypesLookedFor,
		AdditionalTraceParameters
	);

	/// Log what we hit.
	AActor* HitResultActor = HitResult.GetActor();
	if (HitResultActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s detected by LineTrace."), *(HitResultActor->GetName()));
	}
	return HitResult;
}

FVector UGrabber::GetLineTraceStart() const
{
	// Get PlayerViewPoint data.
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);

	return ViewPointLocation;
}

FVector UGrabber::GetLineTraceEnd() const
{
	// Get PlayerViewPoint data.
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT ViewPointLocation, OUT ViewPointRotation);

	FVector LineTraceDirection = ViewPointRotation.Vector(); // (Normalized) unit vector of length 1.

	return ViewPointLocation + LineTraceDirection * LineTraceReach;
}