// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Controller.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Public/CollisionQueryParams.h"
#include "Engine/World.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	
	SetupInputComponent();


	


}





// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{

	FVector PlayeViewPointLocation;
	FRotator PlayerViewPointRotation;
	// get player view point this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayeViewPointLocation,
		OUT PlayerViewPointRotation
	);

	FVector LineTraceEnd = PlayeViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// draw a red line


	/// setup query params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// ray-cast out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters

	);

	/// see what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}

	return HitResult;
}


void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT(" Grab pressed"));

	// LINE TRACE and see if we reach any actors with physics body collision channel set

	auto HitResult = GetFirstPhysicsBodyInReach();
	
	auto ComponentToGrab = HitResult.GetComponent();

	auto ActorHit = HitResult.GetActor();

	// If we hit something then attatch a physics handle
	if (ActorHit)
	{
	
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true//alow rotation
		);

	}

}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT(" Grab released"));
	
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT(" missing component was found"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{/// Look for attached Input component ( only appears at runtime )
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" missing input component was found"), *GetOwner()->GetName());
	}
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayeViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayeViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayeViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayeViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayeViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayeViewPointLocation;
}


