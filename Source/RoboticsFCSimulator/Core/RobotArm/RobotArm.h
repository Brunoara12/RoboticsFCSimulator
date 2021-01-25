// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RoboticsFCSimulator/Core/Pallet/Pallet.h"

#include "RobotArm.generated.h"

UCLASS()
class ROBOTICSFCSIMULATOR_API ARobotArm : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARobotArm();
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	APallet* input;
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	APallet* output;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetInput();
	void SetOutput();
	void InputEmpty();
	void OutputReady();
};
