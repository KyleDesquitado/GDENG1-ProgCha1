// Fill out your copyright notice in the Description page of Project Settings.


#include "Values.h"

float UValues::cMinVehicleActionTime = 1.0f;
float UValues::cMaxVehicleActionTime = 3.0f;
float UValues::cMinVehicleTravelTime = 8.0f;
int UValues::cMaxVehicleLoad = 2;

float UValues::cIronProductionTime = 2.5f;
float UValues::cCoalProductionTime = 2.5f;
float UValues::cFurnaceProductionTime = 5.0f;
float UValues::cLumberProductionTime = 2.5f;
float UValues::cSewingMachineProductionTime = 10.0f;
// Sets default values for this component's properties
UValues::UValues()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UValues::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UValues::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

