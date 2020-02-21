// Copyright Do Over Games All Rights Reserved

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

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

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle){ }
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handle component found on actor: %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("grab pressed"));

	// TODO only ray cast when key is pressed and see if we reach any actors with a physics body collsion channel set
	GetFirstPhysicsBodyInReach();
	// if we hit something then attach the physics handle
	// TODO attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("grab released"));
	// TODO remove/release physics handle
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if physics handle is attached
		// move object
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Parameters for LineTraceSingleByObjectType
	FHitResult Hit;
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	// Ray trace
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	// See what it hits
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Display, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
	}
	return Hit;
}
