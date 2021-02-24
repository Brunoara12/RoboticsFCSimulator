// Fill out your copyright notice in the Description page of Project Settings.


#include "Pallet.h"

#include "UObject/ConstructorHelpers.h"
#include "Product.h"
#include "../HelperFiles/DefinedDebugHelpers.h"

// Sets default values
APallet::APallet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHeight = 180;
	MaxWidth = 100;
	MaxLength = 120;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	MeshComp->SetRelativeScale3D(FVector(1.0, 1.2, .12));
	
	Pallet = FPalletData(MaxHeight / 10, MaxLength / 10, MaxWidth / 10);

	NumOfProductsToFillPallet = 3;
	NumOfProducts = 0;
}

// Called when the game starts or when spawned
void APallet::BeginPlay()
{
	Super::BeginPlay();
	TopLeftCorner = GetActorLocation() + FVector(-MaxWidth / 2, -MaxLength / 2, 6);

	ConstructFullPallet();
}

void APallet::OnConstruction(const FTransform& Transform)
{
	//SetActorLocation(FVector(Transform.GetLocation().X, Transform.GetLocation().Y, 6));

}

void APallet::ConstructFullPallet()
{
	 NumOfProducts = 0;
	while (NumOfProducts < NumOfProductsToFillPallet)
	{
		FVector AvailableSpot = Pallet.GetNextAvailableSpot();
		FVector ProductLocation = 10 * AvailableSpot + TopLeftCorner;
		FTransform TempTran = FTransform(ProductLocation);
		V_LOGM("10x: %s. AS: %s. ActL: %s", *(10 * AvailableSpot).ToString(), *AvailableSpot.ToString(), *GetActorLocation().ToString());
		TempTran.SetRotation(GetActorRotation().Quaternion());

		AProduct* Product = GetWorld()->SpawnActor<AProduct>(AProduct::StaticClass(), TempTran);
		Product->SetActorLocation(ProductLocation +
			FVector(Product->MeshBoxSize.X / 2,
				Product->MeshBoxSize.Y / 2,
				Product->MeshBoxSize.Z / 2));
		Pallet.SetBox(AvailableSpot,
			AvailableSpot + FVector((Product->MeshBoxSize.X / 10) - 1,
				(Product->MeshBoxSize.Y / 10) - 1,
				(Product->MeshBoxSize.Z / 10) - 1), true);
		NumOfProducts++;
		StackOfProducts.Push(Product);
	}
	
}

void APallet::AddProductToPallet()
{
	FVector AvailableSpot = Pallet.GetNextAvailableSpot();
	V_LOGM("%s", *AvailableSpot.ToString());
	if (AvailableSpot.Z != -1)
	{
		FVector ProductLocation = 10 * AvailableSpot + TopLeftCorner;
		FTransform TempTran = FTransform(ProductLocation);
		//V_LOGM("10x: %s. AS: %s. ActL: %s", *(10 * AvailableSpot).ToString(), *AvailableSpot.ToString(), *GetActorLocation().ToString());
		TempTran.SetRotation(GetActorRotation().Quaternion());

		AProduct* Product = GetWorld()->SpawnActor<AProduct>(AProduct::StaticClass(), TempTran);
		
		if (Product)
		{
			Product->SetActorLocation(ProductLocation +
				FVector(Product->MeshBoxSize.X / 2,
					Product->MeshBoxSize.Y / 2,
					Product->MeshBoxSize.Z / 2));
			Pallet.SetBox(AvailableSpot,
				AvailableSpot + FVector((Product->MeshBoxSize.X / 10) - 1,
					(Product->MeshBoxSize.Y / 10) - 1,
					(Product->MeshBoxSize.Z / 10) - 1), true);
			NumOfProducts++;
			StackOfProducts.Push(Product);
		}
	}
}

void APallet::RemoveProductFromPallet()
{
	if (StackOfProducts.Num() > 0) 
	{
		AProduct* Product = StackOfProducts.Pop();

		FVector SpotOnPallet = (Product->GetActorLocation() -
			TopLeftCorner -
			FVector(Product->MeshBoxSize.X / 2,
				Product->MeshBoxSize.Y / 2,
				Product->MeshBoxSize.Z / 2)) / 10;

		V_LOGM("ALoc: %s, Sp: %s", *Product->GetActorLocation().ToString(), *SpotOnPallet.ToString());

		Pallet.SetBox(SpotOnPallet, 
			SpotOnPallet + FVector((Product->MeshBoxSize.X / 10) - 1,
				(Product->MeshBoxSize.Y / 10) - 1,
				(Product->MeshBoxSize.Z / 10) - 1), false);

		Product->Destroy();
		NumOfProducts--;
	}
}

AProduct* APallet::GetProductFromPallet()
{
	if (StackOfProducts.Num() > 0) 
	{
		AProduct* Product = StackOfProducts.Pop();

		FVector SpotOnPallet = (Product->GetActorLocation() -
            TopLeftCorner -
            FVector(Product->MeshBoxSize.X / 2,
                Product->MeshBoxSize.Y / 2,
                Product->MeshBoxSize.Z / 2)) / 10;

		V_LOGM("ALoc: %s, Sp: %s", *Product->GetActorLocation().ToString(), *SpotOnPallet.ToString());

		Pallet.SetBox(SpotOnPallet, 
            SpotOnPallet + FVector((Product->MeshBoxSize.X / 10) - 1,
                (Product->MeshBoxSize.Y / 10) - 1,
                (Product->MeshBoxSize.Z / 10) - 1), false);

		
		NumOfProducts--;
		return Product;
	}
	else
		return nullptr;
}


FVector APallet::GetNextAvailiblePosition()
{
	return  10 * Pallet.GetNextAvailableSpot() + TopLeftCorner;
}

bool APallet::ReadyForNewProduct()
{
	return true;
}

void APallet::StackProductOnPallet(AProduct* Product,FVector Location)
{
	FVector AvailableSpot = Pallet.GetNextAvailableSpot();
	if (Product)
	{
		Pallet.SetBox(AvailableSpot,
            AvailableSpot + FVector((Product->MeshBoxSize.X / 10) - 1,
                (Product->MeshBoxSize.Y / 10) - 1,
                (Product->MeshBoxSize.Z / 10) - 1), true);
		NumOfProducts++;
		StackOfProducts.Push(Product);
	}
	
	
}

// Called every frame
void APallet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DEBUGBOX(GetActorLocation() + FVector(0 ,0, MaxHeight / 2 + 6), FVector(MaxWidth / 2, MaxLength/2, MaxHeight / 2), Blue, 0.2);

}

