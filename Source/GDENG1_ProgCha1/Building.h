// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Building.generated.h"

class UVehicle;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class GDENG1_PROGCHA1_API UBuilding : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuilding();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	

	enum class BuildingType : uint8
	{
		None = 0,
		IronMine = 1,
		CoalMine = 2,
		Furnace = 3,
		Lumberjack = 4,
		SewingMachineFactory = 5
	};


	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	BuildingType buildingType = BuildingType::None;
	UPROPERTY(EditAnywhere) int nBuildingType = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int currentInput1 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int currentInput2 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int maxInput1 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int maxInput2 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int currentOutput = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int maxOutput = 3;

	UPROPERTY(EditAnywhere) float productionTick = 0.0f;
	UPROPERTY(EditAnywhere) float productionTime = 5.0f;
	UPROPERTY(EditAnywhere) bool inQueue = false;

	UPROPERTY(EditAnywhere) AActor* v1Obj;
	UPROPERTY(EditAnywhere) AActor* v2Obj;

	UPROPERTY(EditAnywhere) UVehicle* v1;
	UPROPERTY(EditAnywhere) UVehicle* v2;
};
