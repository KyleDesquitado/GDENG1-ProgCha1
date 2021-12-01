// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"
#include "Values.h"
#include "GameFramework/CharacterMovementComponent.h"

#include <list>
#include "GameFramework/FloatingPawnMovement.h"
#include <xlocale>

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UVehicle::UVehicle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UVehicle::BeginPlay()
{
	Super::BeginPlay();
	vehicleController = (AAIController*)(((APawn*)this->GetOwner())->GetController());

	if(this->destinationObj != nullptr)
	{
		SetDestination(destinationObj);
	}
	if(this->destination != nullptr)
	{
		SetPath(destination->GetOwner());
	}
	// ...
	//this->GetOwner()->FindComponentByClass<UPawnMovementComponent>()->RequestPathMove(destinationObj->GetActorLocation());
	this->minLoadActionTime = UValues::cMinVehicleActionTime;
	this->maxLoadActionTime = UValues::cMaxVehicleActionTime;
	this->minTravelTime = UValues::cMinVehicleTravelTime;
	this->maxLoad = UValues::cMaxVehicleLoad;
	UE_LOG(LogTemp, Warning, TEXT("destinationQueue size :%d"), destinationQueue.Num());
	this->GetOwner()->FindComponentByClass<UCharacterMovementComponent>()->MaxWalkSpeed = 300.0f * (8.0f / minTravelTime);
}




// Called every frame
void UVehicle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if(this->destination != nullptr)
	{
		Travel(DeltaTime);
	}

	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No destination"));
	}*/

	if(destination == nullptr && destinationQueue.Num() >= 1)
	{
		SetDestination(destinationQueue[0]);
	}

	if(vehicleController)
	{
		if(vehicleController->GetPathFollowingComponent()->DidMoveReachGoal() && destination != nullptr)
		{
			switch(this->vehicleState)
			{
			case VehicleState::Moving :
			{

				if ((destination->maxInput1 > 0 && destination->maxInput2 > 0) && (destination->currentInput1 < destination->maxInput1 || destination->currentInput2 < destination->maxInput2))
				{
					//const bool unloadFlag = ((destination->maxInput1 > 0 && destination->maxInput2 > 0) && (destination->currentInput1 < destination->maxInput1 || destination->currentInput2 < destination->maxInput2));
					switch (destination->buildingType)
					{
						{

					case UBuilding::BuildingType::Furnace:
						if(((destination->currentInput1 < destination->maxInput1) && irontCount > 0) || ((destination->currentInput2 < destination->maxInput2) && coalCount > 0))
						{
							vehicleState = VehicleState::Unloading;
							currentLoadActionTime = minLoadActionTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLoadActionTime - minLoadActionTime)));
						}

						else if(destination->currentOutput > 0 && this->currentLoad < this->maxLoad && destination->buildingType != UBuilding::BuildingType::SewingMachineFactory)
						{
							vehicleState = VehicleState::Loading;
							currentLoadActionTime = minLoadActionTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLoadActionTime - minLoadActionTime)));
						}

						else
						{
							vehicleState = VehicleState::Idle;
						}
						/*vehicleState = (irontCount > 0 || coalCount > 0 ? VehicleState::Unloading : VehicleState::Idle);
						if (vehicleState == VehicleState::Unloading)
							currentLoadActionTime = minLoadActionTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLoadActionTime - minLoadActionTime)));*/

						break;
					case UBuilding::BuildingType::SewingMachineFactory:
						if ((destination->currentInput1 < destination->maxInput1) && furnaceCount > 0 || ((destination->currentInput1 < destination->maxInput1) && lumberCount > 0))
						{
							vehicleState = VehicleState::Unloading;
							currentLoadActionTime = minLoadActionTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLoadActionTime - minLoadActionTime)));
						}

						else if (destination->currentOutput > 0 && this->currentLoad < this->maxLoad)
						{
							vehicleState = VehicleState::Loading;
							currentLoadActionTime = minLoadActionTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLoadActionTime - minLoadActionTime)));
						}

						else
						{
							vehicleState = VehicleState::Idle;
						}
						/*vehicleState = (furnaceCount >= 0 || lumberCount > 0 ? VehicleState::Unloading : VehicleState::Idle);
						if (vehicleState == VehicleState::Unloading)
							currentLoadActionTime = minLoadActionTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLoadActionTime - minLoadActionTime)));*/


						break;
					case UBuilding::BuildingType::None:
						UE_LOG(LogTemp, Warning, TEXT("None"));
						currentLoadActionTime = 0.0f;
						break;
						}
					}

					if (vehicleState == VehicleState::Unloading)
					{
						UE_LOG(LogTemp, Warning, TEXT("Transition to: Unloading	from: Moving"));
						UE_LOG(LogTemp, Warning, TEXT("Randomized Load Action Time: %f"), currentLoadActionTime);
					}

					else if (vehicleState == VehicleState::Idle)
					{
						UE_LOG(LogTemp, Warning, TEXT("Transition to: Idle	from: Moving"));
					}

				}

				else if ((destination->currentOutput > 0 && this->currentLoad < this->maxLoad) && destination->buildingType != UBuilding::BuildingType::SewingMachineFactory)
				{
					vehicleState = VehicleState::Loading;
					currentLoadActionTime = minLoadActionTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLoadActionTime - minLoadActionTime)));
					UE_LOG(LogTemp, Warning, TEXT("Transition to: Loading	from: Moving"));
					UE_LOG(LogTemp, Warning, TEXT("Randomized Load Action Time: %f"), currentLoadActionTime);
				}

				else
				{
					vehicleState = VehicleState::Idle;
					UE_LOG(LogTemp, Warning, TEXT("Transition to: Idle	from: Moving"));
				}
			}
			break;

			case VehicleState::Loading:
			{
				UE_LOG(LogTemp, Warning, TEXT("Before: Vehicle State: Loading"));

				if(currentLoadActionTick < currentLoadActionTime)
				{
					currentLoadActionTick += DeltaTime;
					UE_LOG(LogTemp, Warning, TEXT("Tick: %f	/	Time: %f"), currentLoadActionTick, currentLoadActionTime);
					UE_LOG(LogTemp, Warning, TEXT("if"));
				}

				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Loading time reached"))
					LoadItem(destination);
					
					currentLoadActionTick = 0.0f;
					
				}

				


				if(vehicleState == VehicleState::Loading)
				UE_LOG(LogTemp, Warning, TEXT("After: Vehicle State: Loading"));
					
			}
			break;


			case VehicleState::Unloading:
			{
				if (currentLoadActionTick >= currentLoadActionTime)
				{
					currentLoadActionTick = 0.0f;
					UnloadItem(destination);
				}

				else
					currentLoadActionTick += DeltaTime;
			}
			break;

			}

		
			if (vehicleState == VehicleState::Idle)
			{
				RemoveDestination();
			}
				
		}
	}

	
	// ...
}

void UVehicle::SetDestination(AActor* newDestination)
{
	this->destination = newDestination->FindComponentByClass<UBuilding>();

	if (vehicleController)
	{
		if(!HasInventorySpace())
		{
			if(destination->maxInput1 > 0 && destination->maxInput2 > 0)
			{
				bool shouldContinue1 = false;
				bool shouldContinue2 = false;
				switch(destination->buildingType)
				{
					case UBuilding::BuildingType::Furnace:
						shouldContinue1 = (this->irontCount > 0 && destination->currentInput1 < destination->maxInput1) || (this->coalCount > 0 && destination->currentInput2 < destination->maxInput2);
						(shouldContinue1 ? vehicleController->MoveToActor(destination->GetOwner(), 0.0f, true) : RemoveDestination());
						
						break;

					case UBuilding::BuildingType::SewingMachineFactory:
						shouldContinue2 = (this->furnaceCount > 0 && destination->currentInput1 < destination->maxInput1) || (this->lumberCount > 0 && destination->currentInput2 < destination->maxInput2);
						(shouldContinue2 ? vehicleController->MoveToActor(destination->GetOwner(), 0.0f, true) : RemoveDestination());

						break;

					case UBuilding::BuildingType::None: UE_LOG(LogTemp, Warning, TEXT("None")); break;
				}

				if(shouldContinue1 || shouldContinue2)
				{
					vehicleState = VehicleState::Moving;
				}

			}

			else
			{
				RemoveDestination();
			}
		}

		else
		{
			vehicleController->MoveToActor(destination->GetOwner(), 0.0f, true);
			vehicleState = VehicleState::Moving;
			UE_LOG(LogTemp, Warning, TEXT("destination set"));
		}
		
	}


	else
		UE_LOG(LogTemp, Warning, TEXT("no ai controller"))

	

}

void UVehicle::SetDestination(UBuilding* newDestination)
{
	this->destination = newDestination;

	if (vehicleController)
	{
		if (!HasInventorySpace())
		{
			if (destination->maxInput1 > 0 && destination->maxInput2 > 0)
			{
				bool shouldContinue1 = false;
				bool shouldContinue2 = false;
				switch (destination->buildingType)
				{
				case UBuilding::BuildingType::Furnace:
					shouldContinue1 = (this->irontCount > 0 && destination->currentInput1 < destination->maxInput1) || (this->coalCount > 0 && destination->currentInput2 < destination->maxInput2);
					(shouldContinue1 ? vehicleController->MoveToActor(destination->GetOwner(), 0.0f, true) : RemoveDestination());

					break;

				case UBuilding::BuildingType::SewingMachineFactory:
					shouldContinue2 = (this->furnaceCount > 0 && destination->currentInput1 < destination->maxInput1) || (this->lumberCount > 0 && destination->currentInput2 < destination->maxInput2);
					(shouldContinue2 ? vehicleController->MoveToActor(destination->GetOwner(), 0.0f, true) : RemoveDestination());

					break;

				case UBuilding::BuildingType::None: UE_LOG(LogTemp, Warning, TEXT("None")); break;
				}

				if (shouldContinue1 || shouldContinue2)
				{
					vehicleState = VehicleState::Moving;
				}

			}

			else
			{
				RemoveDestination();
			}
		}

		else
		{
			vehicleController->MoveToActor(destination->GetOwner(), 0.0f, true);
			vehicleState = VehicleState::Moving;
			UE_LOG(LogTemp, Warning, TEXT("destination set"));
		}

	}


	else
		UE_LOG(LogTemp, Warning, TEXT("no ai controller"))

}

void UVehicle::RemoveDestination()
{
	destination->inQueue = false;
	destinationQueue.Remove(destination);
	destination = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("end"));
}

void UVehicle::DelayDestination()
{
	UBuilding* destinationPtr = destination;
	destinationQueue.Remove(destination);
	destination = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("delayed"));
	destinationQueue.Add(destinationPtr);

}

void UVehicle::AddAsDestination(UBuilding* newDestination)
{
	destinationQueue.Emplace(newDestination);
}

void UVehicle::SetPath(AActor* newDestination)
{
	start = this->GetOwner()->GetActorLocation();
	end = newDestination->GetActorLocation();
}

void UVehicle::Travel(float deltaTime)
{
	float speed = 1 / minTravelTime;

	float alpha = (travelDuration / minTravelTime);
	if (alpha > 1) alpha = 1;

	FVector pos = FMath::Lerp(start, end, alpha);

	this->GetOwner()->SetActorLocation(pos);


	if (pos == end)
	{
		int oldLoad = this->currentLoad;
		int newLoad = oldLoad;

		while(newLoad < maxLoad && destination->currentOutput > 0)
		{
			newLoad++;
			destination->currentOutput--;
		}

		switch(destination->buildingType)
		{
			{
		case UBuilding::BuildingType::IronMine: irontCount++; break;
		case UBuilding::BuildingType::CoalMine:coalCount++; break;
		case UBuilding::BuildingType::Furnace: furnaceCount++; break;
		case UBuilding::BuildingType::Lumberjack: lumberCount++; break;
		case UBuilding::BuildingType::SewingMachineFactory: break;
		case UBuilding::BuildingType::None: UE_LOG(LogTemp, Warning, TEXT("None")); break;
			}
		}

		if(destination->buildingType != UBuilding::BuildingType::SewingMachineFactory)
		currentLoad++;

		destinationObj = nullptr;
		destination = nullptr;
		travelDuration = 0.0f;
	}

	/*FVector pos = this->GetOwner()->GetActorLocation();
	FVector dir = (destination->GetActorLocation() - pos).GetSafeNormal();
	dir *= speed;
	FVector newPos = pos + dir;
	this->GetOwner()->SetActorLocation(newPos);

	if(pos.Dist(pos, destination->GetActorLocation()) < 100)
	{	
		destination = nullptr;
	}*/

	travelDuration += deltaTime;
	UE_LOG(LogTemp, Warning, TEXT("Alpha: %f, Travel Duration: %f"), alpha, travelDuration);
}

void UVehicle::LoadItem(UBuilding* target)
{
	UE_LOG(LogTemp, Warning, TEXT("In loading function"));
	/*int* itemLoad = &(this->currentLoad);
	int* itemStock = &(target->currentOutput);
	int addedLoad = 0;

	while (*itemLoad < maxLoad && target->currentOutput > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading Item, item count: %d"), *itemStock);
		addedLoad++;
		(*itemLoad)++;
		target->currentOutput--;
	}*/

	switch (target->buildingType)
	{
			
		case UBuilding::BuildingType::IronMine: this->irontCount++; break;
		case UBuilding::BuildingType::CoalMine:this->coalCount++; break;
		case UBuilding::BuildingType::Furnace: this->furnaceCount++; break;
		case UBuilding::BuildingType::Lumberjack: this->lumberCount++; break;
		case UBuilding::BuildingType::SewingMachineFactory: break;
		case UBuilding::BuildingType::None: UE_LOG(LogTemp, Warning, TEXT("Invalid building type in LoadItem function")); break;
			
	}

	if (destination->buildingType != UBuilding::BuildingType::SewingMachineFactory)
	{
		currentLoad++;
		target->currentOutput--;
	}

	else
	{
		vehicleState = VehicleState::Idle;
	}

	if (this->currentLoad == this->maxLoad || destination->currentOutput == 0)
	{
		vehicleState = VehicleState::Idle;
	}
	
}

void UVehicle::UnloadItem(UBuilding* target)
{
	UE_LOG(LogTemp, Warning, TEXT("In unloading function"));
	int* itemLoad1 = new int(0);
	int* itemLoad2 = new int(0);
	switch (target->buildingType)
	{
		{
	case UBuilding::BuildingType::Furnace:
		itemLoad1 = &irontCount;
		itemLoad2 = &coalCount;
			break;
	case UBuilding::BuildingType::SewingMachineFactory:
		itemLoad1 = &furnaceCount;
		itemLoad2 = &lumberCount;
			; break;
	case UBuilding::BuildingType::None:
		UE_LOG(LogTemp, Warning, TEXT("Invalid building type in UnloadItem function"));
		break;
		}
	}

	if(destination->currentInput1 < destination->maxInput1 &&  *itemLoad1 > 0)
	{
		currentLoad--;
		(*itemLoad1)--;
		target->currentInput1++;
	}

	else if(destination->currentInput2 < destination->maxInput2 && *itemLoad2 > 0)
	{
		currentLoad--;
		(*itemLoad2)--;
		target->currentInput2++;
	}

	else if ((destination->currentOutput > 0 && this->currentLoad < this->maxLoad) && destination->buildingType != UBuilding::BuildingType::SewingMachineFactory)
	{
		vehicleState = VehicleState::Loading;
		currentLoadActionTime = minLoadActionTime + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (maxLoadActionTime - minLoadActionTime)));
		UE_LOG(LogTemp, Warning, TEXT("Transition to: Loading	from: Unloading"));
		UE_LOG(LogTemp, Warning, TEXT("Randomized Load Action Time: %f"), currentLoadActionTime);
	}

	else
	{
		vehicleState = VehicleState::Idle;
	}
}

bool UVehicle::HasInventorySpace()
{
	return (this->currentLoad < this->maxLoad);
}
bool UVehicle::HasBuildingNeeds(UBuilding* target)
{
	bool hasBuildingNeeds = false;
	bool hasNeeds1 = false;
	bool hasNeeds2 = false;
	switch (target->buildingType)
	{
		case UBuilding::BuildingType::Furnace: 
			hasNeeds1 = (target->currentInput1 < target->maxInput1&& this->irontCount > 0);
			hasNeeds2 = (target->currentInput2 < target->maxInput2&& this->coalCount> 0);
			
			break;
		case UBuilding::BuildingType::SewingMachineFactory:
			hasNeeds1 = (target->currentInput1 < target->maxInput1&& this->furnaceCount > 0);
			hasNeeds2 = (target->currentInput2 < target->maxInput2&& this->lumberCount> 0);
			
			break;
		case UBuilding::BuildingType::None: UE_LOG(LogTemp, Warning, TEXT("None")); break;
	}

	hasBuildingNeeds = hasNeeds1 || hasNeeds2;

	return hasBuildingNeeds;
}

int UVehicle::getQueueSize()
{
	return destinationQueue.Num();
}
