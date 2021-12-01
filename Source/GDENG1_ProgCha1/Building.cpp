// Fill out your copyright notice in the Description page of Project Settings.

#include "Values.h"
#include "Vehicle.h"
#include "Building.h"

// Sets default values for this component's properties
UBuilding::UBuilding()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UBuilding::BeginPlay()
{
	Super::BeginPlay();

	this->buildingType = static_cast<BuildingType>(nBuildingType);

	switch(buildingType)
	{
	case BuildingType::IronMine: this->productionTime = UValues::cIronProductionTime; break;
	case BuildingType::CoalMine: this->productionTime = UValues::cCoalProductionTime; break;
	case BuildingType::Furnace: this->productionTime = UValues::cFurnaceProductionTime; break;
	case BuildingType::Lumberjack: this->productionTime = UValues::cLumberProductionTime; break;
	case BuildingType::SewingMachineFactory: this->productionTime = UValues::cSewingMachineProductionTime; break;
	case BuildingType::None: UE_LOG(LogTemp, Warning, TEXT("None")); break;
	}

	
	// ...
	v1 = v1Obj->FindComponentByClass<UVehicle>();
	v2 = v2Obj->FindComponentByClass<UVehicle>();
}


// Called every frame
void UBuilding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(this->currentOutput < this->maxOutput)
	{
		if(this->maxInput1 == 0 && this->maxInput2 == 0)
			this->productionTick += DeltaTime;

		else if((this->maxInput1 > 0 && this->maxInput2 > 0) && (this->currentInput1 > 0 && this->currentInput2 > 0))
			this->productionTick += DeltaTime;
	}
		



	bool hasOutput = currentOutput > 0;
	bool needsInput = false;

	switch(this->buildingType)
	{
	case BuildingType::IronMine:
	case BuildingType::CoalMine:
	case BuildingType::Lumberjack:
	
		if ((this->productionTick >= this->productionTime) && (this->currentOutput < this->maxOutput))
		{
			currentOutput++;
			productionTick = 0.0f;
		}

		break;

	case BuildingType::Furnace:
	case BuildingType::SewingMachineFactory:

		if(this->currentInput1 > 0 && this->currentInput2 > 0)
		{
			if ((this->productionTick >= this->productionTime) && (this->currentOutput < this->maxOutput))
			{
				currentOutput++;
				currentInput1--;
				currentInput2--;
				productionTick = 0.0f;
			}
		}

		break;
		
	}

	if(currentOutput >= 1 && inQueue == false)
	{
		if (v1 && v2)
		{
			if (v1->getQueueSize() <= v2->getQueueSize() && v1->HasInventorySpace())
			{
				v1->AddAsDestination(this);
				inQueue = true;
			}

			else if (v2->getQueueSize() <= v1->getQueueSize() && v2->HasInventorySpace())
			{
				v2->AddAsDestination(this);
				inQueue = true;
			}

			if (v1->HasInventorySpace() == false && v1->HasBuildingNeeds(this))
			{
				v1->AddAsDestination(this);
				inQueue = true;
			}

			else if (this->buildingType == BuildingType::Furnace)
			{
				if (!(v1->HasInventorySpace()) && v1->irontCount > 0 && this->currentInput1 == this->maxInput1 && productionTick == 0.0f)
				{
					v1->currentLoad -= v1->irontCount;
					v1->irontCount = 0;
				}

				else if (!(v1->HasInventorySpace()) && v1->coalCount > 0 && this->currentInput2 == this->maxInput2 && productionTick == 0.0f)
				{
					v1->currentLoad -= v1->coalCount;
					v1->coalCount = 0;
				}
			}

			else if (this->buildingType == BuildingType::SewingMachineFactory)
			{
				if (!(v1->HasInventorySpace()) && v1->furnaceCount > 0 && this->currentInput1 == this->maxInput1 && productionTick == 0.0f)
				{
					v1->currentLoad--;
					v1->furnaceCount--;
				}

				if (!(v1->HasInventorySpace()) && v1->lumberCount > 0 && this->currentInput2 == this->maxInput2 && productionTick == 0.0f)
				{
					v1->currentLoad -= v1->lumberCount;
					v1->lumberCount = 0;
				}
			}
			



			if (v2->HasInventorySpace() == false && v2->HasBuildingNeeds(this))
			{
				v2->AddAsDestination(this);
				inQueue = true;
			}

			else if (this->buildingType == BuildingType::Furnace)
			{
				if (!(v2->HasInventorySpace()) && v2->irontCount > 0 && this->currentInput1 == this->maxInput1 && productionTick == 0.0f)
				{
					v2->currentLoad -= v2->irontCount;
					v2->irontCount = 0;
				}

				else if (!(v2->HasInventorySpace()) && v2->coalCount > 0 && this->currentInput2 == this->maxInput2 && productionTick == 0.0f)
				{
					v2->currentLoad -= v2->coalCount;
					v2->coalCount = 0;
				}
			}

			else if (this->buildingType == BuildingType::SewingMachineFactory)
			{
				if (!(v2->HasInventorySpace()) && v2->furnaceCount > 0 && this->currentInput1 == this->maxInput1 && productionTick == 0.0f)
				{
					v2->currentLoad--;
					v2->furnaceCount--;
				}

				if (!(v2->HasInventorySpace()) && v2->lumberCount > 0 && this->currentInput2 == this->maxInput2 && productionTick == 0.0f)
				{
					v2->currentLoad -= v2->lumberCount;
					v2->lumberCount = 0;
				}
			}
		}

		else if(v1)
		{
			if(v1->HasInventorySpace())
			{
				v1->AddAsDestination(this);
				inQueue = true;
			}
		}

		else if(v2)
		{
			if (v2->HasInventorySpace())
			{
				v2->AddAsDestination(this);
				inQueue = true;
			}
			
		}

		

		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No vehicles"));
		}
	}

	else if(inQueue == false)
	{
		if (v1 && v2)
		{
			if (v1->HasInventorySpace() == false && v1->HasBuildingNeeds(this))
			{
				v1->AddAsDestination(this);
				inQueue = true;
			}

			else if (this->buildingType == BuildingType::Furnace)
			{
				if (!(v1->HasInventorySpace()) && v1->irontCount > 0 && this->currentInput1 == this->maxInput1 && productionTick == 0.0f)
				{
					v1->currentLoad -= v1->irontCount;
					v1->irontCount = 0;
				}

				else if (!(v1->HasInventorySpace()) && v1->coalCount > 0 && this->currentInput2 == this->maxInput2 && productionTick == 0.0f)
				{
					v1->currentLoad -= v1->coalCount;
					v1->coalCount = 0;
				}
			}

			else if (this->buildingType == BuildingType::SewingMachineFactory)
			{

				if (!(v1->HasInventorySpace()) && v1->furnaceCount > 0 && this->currentInput1 == this->maxInput1 && productionTick == 0.0f)
				{
					v1->currentLoad--;
					v1->furnaceCount--;
				}

				if (!(v1->HasInventorySpace()) && v1->lumberCount > 0 && this->currentInput2 == this->maxInput2 && productionTick == 0.0f)
				{
					v1->currentLoad -= v1->lumberCount;
					v1->lumberCount = 0;
				}
			}

			if (v2->HasInventorySpace() == false && v2->HasBuildingNeeds(this))
			{
				v2->AddAsDestination(this);
				inQueue = true;
			}

			else if (this->buildingType == BuildingType::Furnace)
			{
				if (!(v2->HasInventorySpace()) && v2->irontCount > 0 && this->currentInput1 == this->maxInput1 && productionTick == 0.0f)
				{
					v2->currentLoad -= v2->irontCount;
					v2->irontCount = 0;
				}

				else if (!(v2->HasInventorySpace()) && v2->coalCount > 0 && this->currentInput2 == this->maxInput2 && productionTick == 0.0f)
				{
					v2->currentLoad -= v2->coalCount;
					v2->coalCount = 0;
				}
			}

			else if (this->buildingType == BuildingType::SewingMachineFactory)
			{
				if (!(v2->HasInventorySpace()) && v2->furnaceCount > 0 && this->currentInput1 == this->maxInput1 && productionTick == 0.0f)
				{
					v2->currentLoad--;
					v2->furnaceCount--;
				}

				if (!(v2->HasInventorySpace()) && v2->lumberCount > 0 && this->currentInput2 == this->maxInput2 && productionTick == 0.0f)
				{
					v2->currentLoad -= v2->lumberCount;
					v2->lumberCount = 0;
				}
			}
		}

		else if(v1)
		{
			if(v1->HasInventorySpace() == false && v1->HasBuildingNeeds(this))
			{
				v1->AddAsDestination(this);
				inQueue = true;
			}
		}

		else if (v2)
		{
			if (v2->HasInventorySpace() == false && v2->HasBuildingNeeds(this))
			{
				v2->AddAsDestination(this);
				inQueue = true;
			}
		}

		
	}


	// ...
}

