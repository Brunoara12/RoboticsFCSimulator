// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageRack.h"

// Sets default values
AStorageRack::AStorageRack()
{
	
	PrimaryActorTick.bCanEverTick = true;
	
}
// Called when the game starts or when spawned
void AStorageRack::BeginPlay()
{
	Super::BeginPlay();
	Offsets[0] = FVector(0,0,20);
	Offsets[1] = FVector(0,175,20);
	Offsets[2] = FVector(0,-175,20);
	Offsets[3] = FVector(0,0,310);
	Offsets[4] = FVector(0,175,310);
	Offsets[5] = FVector(0,-175,310);
	
}

// Called every frame
void AStorageRack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Check_Pallets();

}

FVector AStorageRack::Get_Available_Space()
{
	for( int i = 0; i < 6; ++i)
	{
		if(Pallets[i] == nullptr)
		{
			return RootComponent->GetComponentLocation() + Offsets[i];
		}
		
	}
	return FVector(0,0,0);
}

APallet* AStorageRack::Pop_Pallet()
{
	for( int i = 0; i < 6; ++i)
	{
		if(Pallets[i] != nullptr)
		{
			APallet* retVal = Pallets[i];
			Pallets[i] = nullptr;
			return retVal;
		}
		
	}
	return nullptr;
}

void AStorageRack::Put_Pallet(APallet* input)
{
	for( int i = 0; i < 6; ++i)
	{
		if(Pallets[i] == nullptr)
		{
			Pallets[i] = input;
		}
		
	}
	
}

void AStorageRack::Check_Pallets()
{
	for( int i = 0; i < 6; ++i)
	{
		if(Pallets[i] != nullptr)
		{
			if(Pallets[i]->GetActorLocation() != GetActorLocation() + Offsets[i])
			{
				Pallets[i]->SetActorLocation(GetActorLocation() + Offsets[i]);
				
			}
		}
		
	}
}

