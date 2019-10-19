// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "OpenDoor.h"

// Points out variable modification by certain functions when passed in as argument.
#define OUT // compiles to nothing. 

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ensureMsgf(PressurePlate != nullptr, TEXT("%s: PressurePlate not found."), *GetOwner()->GetName());
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume for trigger events.
	if (GetTotalMassOfActorsOnPlate() > TriggerMassThreshold) 
	{
		// Broadcast request to blueprint.
		OnOpenRequest.Broadcast();
	}
	else
	{
		// Broadcast request to blueprint.
		OnCloseRequest.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	// Find all the overlapping actors.
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses.
	for (const auto* Actor : OverlappingActors)
	{
		UE_LOG
		(
			LogTemp, Warning, 
			TEXT("%s: detected overlapping actor: %s."), 
			*GetOwner()->GetName(), 
			*Actor->GetName()
		);
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}
