// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Values.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GDENG1_PROGCHA1_API UValues : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UValues();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static float cMinVehicleActionTime;
	static float cMaxVehicleActionTime;
	static float cMinVehicleTravelTime;
	static int cMaxVehicleLoad;

	static float cIronProductionTime;
	static float cCoalProductionTime;
	static float cFurnaceProductionTime;
	static float cLumberProductionTime;
	static float cSewingMachineProductionTime;
};
