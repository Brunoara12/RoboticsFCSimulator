// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotArm.h"

// Sets default values
ARobotArm::ARobotArm()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARobotArm::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobotArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARobotArm::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARobotArm::SetInput()
{
	
}

void ARobotArm::SetOutput()
{
}

void ARobotArm::InputEmpty()
{
}

void ARobotArm::OutputReady()
{
}

