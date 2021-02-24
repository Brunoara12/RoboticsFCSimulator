// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Pallet/Pallet.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StorageRack.generated.h"


UCLASS()
class ROBOTICSFCSIMULATOR_API AStorageRack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStorageRack();
	
protected:
	UPROPERTY(EditAnywhere)
	APallet* Pallets[6];
	UPROPERTY(EditAnywhere)
	FVector Offsets[6];
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FVector Get_Available_Space();

	virtual APallet* Pop_Pallet();

	virtual void Put_Pallet(APallet* input);

	virtual void Check_Pallets();

};
