// Fill out your copyright notice in the Description page of Project Settings.


#include "StagingArea.h"
#include "Engine/TargetPoint.h"
#include "../HelperFiles/DefinedDebugHelpers.h"
#include "Pallet.h"

// Sets default values
AStagingArea::AStagingArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	

	MeshComp->SetRelativeScale3D(FVector(11.5, 6.1, 1));

	static ConstructorHelpers::FClassFinder<APallet> PalletBPClass(TEXT("/Game/Core/Pallet/BP_Pallet"));
	if (PalletBPClass.Class != nullptr)
	{
		PalletClass = PalletBPClass.Class;
	}

	StagingData = FStagingData();
}

// Called when the game starts or when spawned
void AStagingArea::BeginPlay()
{
	Super::BeginPlay();

	
	FTransform TargetTransform = StagingData.GetSpawnTransform(1);
	if (TargetTransform.GetLocation().Z != -1)
	{
		APallet* SpawnPallet = GetWorld()->SpawnActor<APallet>(PalletClass, TargetTransform);
		StagingData.AddPalletToLane(1, SpawnPallet);
	}
}

void AStagingArea::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetActorLocation(FVector(Transform.GetLocation().X, Transform.GetLocation().Y, 1));

	if (!HasAllFlags(RF_Transient))
	{
		TopLeftCorner = Transform.GetLocation() + FVector(-1150 / 2, -610 / 2, 0);

		FVector TargetLocation;
		FTransform TempTran;
		StagingData.Clear();
		StagingData.test++;
		V_LOG("HI");
		V_LOGM("Lane %d + Points %d", StagingData.Lanes.Num(), StagingData.Lanes[0].SpawnPointVectors.Num());
		if (StagingData.Lanes[0].SpawnPointVectors.Num() == 0)
		{
			for (int i = 0; i < StagingData.Lanes.Num(); i++)
			{
				StagingData.Lanes[i].CreateSpawnPoints();
			}
		}
		for (int i = 0; i < StagingData.Lanes.Num(); i++)
		{
			for (int j = 0; j < StagingData.Lanes[i].SpawnPointVectors.Num(); j++)
			{
				V_LOGM("HERE: %d %d", i, j);
				StagingData.Lanes[i].t++;

				TargetLocation = TopLeftCorner + StagingData.Lanes[i].SpawnPointVectors[j];
				TempTran = FTransform(FRotator(0,90,0), TargetLocation);
				ATargetPoint* SpawnTP = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), TempTran);
 				StagingData.Lanes[i].SpawnTargetPoint(SpawnTP);
			}
		}
	}
	
}

void AStagingArea::BeginDestroy()
{

	Super::BeginDestroy();

	//StagingData.Clear();
	//for (int i = 0; i < StagingData.Lanes.Num(); i++)
	//{
	//	for (int j = 0; j < StagingData.Lanes[i].SpawnPoints.Num(); j++)
	//	{
	//		if (StagingData.Lanes[i].SpawnPoints[j] != nullptr)
	//		{
	//			StagingData.Lanes[i].SpawnPoints[j]->Destroy();
	//		}
	//	}
	//	StagingData.Lanes[i].SpawnPoints.Empty();
	//}
}

// Called every frame
void AStagingArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

