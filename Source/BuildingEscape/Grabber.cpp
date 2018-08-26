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

	UE_LOG(LogTemp, Warning, TEXT("grabber reporting for duty"));
	
	/// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//physics handle is found
		UE_LOG(LogTemp, Warning, TEXT("Component is included"));

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" missing component was found"), *GetOwner()->GetName());
	}


	/// Look for attached Input component ( only appears at runtime )
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		//physics handle is found
		UE_LOG(LogTemp, Warning, TEXT("input Component is included"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" missing input component was found"), *GetOwner()->GetName());
	}


}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT(" Grab pressed"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT(" Grab released"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayeViewPointLocation;
	FRotator PlayerViewPointRotation;
	// get player view point this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayeViewPointLocation,
		OUT PlayerViewPointRotation
	);


	// TODO log out to test
	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
		*PlayeViewPointLocation.ToString() ,
		*PlayerViewPointRotation.ToString()
	);*/

	FVector LineTraceEnd = PlayeViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// draw a red line

	DrawDebugLine(
		GetWorld(),
		PlayeViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);
	/// setup query params
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayeViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters

		);

	/// see what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}
}

