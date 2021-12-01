// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclaAI.h"

// Sets default values
AVehiclaAI::AVehiclaAI()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVehiclaAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVehiclaAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVehiclaAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

