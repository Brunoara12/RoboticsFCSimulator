// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "RoboticsFCSimulator/Core/Pallet/Pallet.h"
#include "RoboticsFCSimulator/Core/Conveyor/ConveyorBelt.h"
#include "Components/SphereComponent.h"
#include "Math/UnrealMathVectorCommon.h"

#include "RobotArm.generated.h"

UCLASS()
class ROBOTICSFCSIMULATOR_API ARobotArm : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARobotArm();
protected:
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	class APallet* input;
	UPROPERTY(editAnywhere, BlueprintReadWrite)
	class AConveyorBelt* output;
	UPROPERTY(editAnywhere)
	class AProduct* currentProduct;
	UPROPERTY(editAnywhere)
	class USphereComponent* SphereCom;
	UPROPERTY(editAnywhere)
	FVector dropPoint;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
    void Transfer();
	void DropProduct();
	UFUNCTION(BlueprintCallable)
	bool InputEmpty();
	UFUNCTION(BlueprintCallable)
	bool OutputReady();
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* newComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
