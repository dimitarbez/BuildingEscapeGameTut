// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/Controller.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
		*PlayeViewPointLocation.ToString() ,
		*PlayerViewPointRotation.ToString()
	);

	// ray-cast out to reach distance

	// see what we hit
}

