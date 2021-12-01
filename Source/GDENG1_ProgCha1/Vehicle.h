// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIController.h"
#include "Building.h"
#include "Vehicle.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType )
class GDENG1_PROGCHA1_API UVehicle : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVehicle();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void RemoveDestination();
	void DelayDestination();

public:
	

	enum class VehicleState : uint8
	{
		Idle = 0,
		Moving = 1,
		Loading = 2,
		Unloading = 3,
	};

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetDestination(AActor* newDestination);
	void SetDestination(UBuilding* newDestination);
	void AddAsDestination(UBuilding* newDestination);
	void SetPath(AActor* newDestination);
	void Travel(float deltaTime);
	void LoadItem(UBuilding* target);
	void UnloadItem(UBuilding* target);
	bool HasInventorySpace();
	bool HasBuildingNeeds(UBuilding* target);
	int getQueueSize();
	UPROPERTY(EditAnywhere) AActor* destinationObj;

	AAIController* vehicleController;

	VehicleState vehicleState = VehicleState::Idle;


	UPROPERTY(EditAnywhere) float minTravelTime = 4.0f;
	UPROPERTY(EditAnywhere) float travelDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int currentLoad = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int maxLoad = 4;

	UPROPERTY(EditAnywhere) float minLoadActionTime = 1;
	UPROPERTY(EditAnywhere) float maxLoadActionTime = 3;
	UPROPERTY(EditAnywhere) float currentLoadActionTime;
	UPROPERTY(EditAnywhere) float currentLoadActionTick = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int irontCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int coalCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int furnaceCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int lumberCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int sewingMachineCount = 0;

	TArray<UBuilding*> destinationQueue;
	UPROPERTY(EditAnywhere) UBuilding* destination;
	FVector start;
	FVector end;
		
};
