// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pallet.h"
#include "Engine/TargetPoint.h"

#include "StagingArea.generated.h"


USTRUCT(BlueprintType)
struct FLaneData {
	GENERATED_BODY()

		int OFFSET_SpaceX = 200;
	int OFFSET_PalletX = 120;
	int OFFSET_PalletY = 100;
	int OFFSET_Padding = 10;
	int OFFSET_Border = 25;

	/*****************
	S	P	P	S	P	5	S
	S	P	P	S	P	4	S
	S	P	P	S	P	3	S
	S	P	P	S	P	2	S
	S	P	P	S	P	1	S
	*******************/

public:
	// Pallet 1- 5 in Lane
	TArray<class APallet*> PalletsInLane;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray< ATargetPoint*> SpawnPoints;

	TArray<FVector> SpawnPointVectors;

	FString LaneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 t = 0;

	// 1 - 4
	int32 LaneIndex;
	int8 NumOfPallets;

	void Clear()
	{
		for (int i = 0; i < SpawnPoints.Num(); i++)
		{
			SpawnPoints[i]->Destroy();
			
		}
		SpawnPoints.Empty();
	}

	FTransform GetSpawnTransform()
	{
		
		if (NumOfPallets < 5)
		{
			return  SpawnPoints[NumOfPallets]->GetTransform();
		}
		return FTransform(FVector(-1,-1,-1));
	}

	bool AddPalletToLane(APallet* Pallet)
	{
		if (PalletsInLane.Num() < 5)
		{
			PalletsInLane.Add(Pallet);
			NumOfPallets++;
			return true;
		}
		return false;
	}

	APallet* RemovePalletFromLane()
	{
		if (PalletsInLane.Num() > 0)
		{
			NumOfPallets--;
			return PalletsInLane.Pop();
		}

		return nullptr;
	}

	void CreateSpawnPoints()
	{
		
		int32 OFFSET_X = OFFSET_Border + 
			(4 - LaneIndex) * OFFSET_PalletX +
			(LaneIndex <= 2 ? 2 * OFFSET_SpaceX : 1 * OFFSET_SpaceX) +
			(4 - LaneIndex + 1) / 2 * OFFSET_Padding +
			OFFSET_PalletX / 2;

		for (int i = 0; i < 5; i++)
		{
			int32 OFFSET_Y = OFFSET_Border +
				(4 - i) * OFFSET_PalletY +
				(5 - i) * OFFSET_Padding +
				OFFSET_PalletY / 2;

			SpawnPointVectors.Add(FVector(OFFSET_X, OFFSET_Y, 60));
		}
	}

	void SpawnTargetPoint(ATargetPoint* SpawnTP)
	{
		SpawnPoints.Add(SpawnTP);
	}

	FLaneData()
	{

	}

	FLaneData(FString LName, int32 LIndex)
	{
		LaneName = LName;
		LaneIndex = LIndex;
		NumOfPallets = 0;

		CreateSpawnPoints();
	}

	~FLaneData()
	{
		SpawnPointVectors.Empty();
	}
};

USTRUCT(BlueprintType)
struct FStagingData {
	GENERATED_BODY()


	/*****************
	S	P	P	S	P	P	S
	S	P	P	S	P	P	S
	S	P	P	S	P	P	S
	S	P	P	S	P	P	S
	S	P	P	S	P	P	S
		4   3		2   1
	*******************/
public:
	// Lanes 1 - 4
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FLaneData> Lanes;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 test = 0;

	FTransform GetSpawnTransform(int LIndex)
	{
		return Lanes[LIndex-1].GetSpawnTransform();
	}

	bool AddPalletToLane(int LIndex, APallet* pallet)
	{
		return Lanes[LIndex-1].AddPalletToLane(pallet);
	}

	// Clear TargetPoints
	void Clear()
	{
		
		for (int i = 0; i < Lanes.Num(); i++)
		{
			Lanes[i].Clear();
		}
	}

	FStagingData() {
		Clear();
		for (int i = 0; i < 4; i++)
		{

			Lanes.Add(FLaneData(FString::FromInt(i), i + 1));
		}
	}
};

UCLASS()
class ROBOTICSFCSIMULATOR_API AStagingArea : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStagingArea();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StagingData, meta = (AllowPrivateAccess = "true"))
	FStagingData StagingData;

	int32 NumOfPallets;

	TArray<class APallet*> PalletsInStagingArea;

	TArray<class ATargetPoint* > TargetPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginDestroy() override;

	FVector TopLeftCorner;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
