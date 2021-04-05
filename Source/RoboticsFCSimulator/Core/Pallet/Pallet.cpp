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
	FVector CornerOffset = GetActorRotation().RotateVector(FVector(-MaxWidth / 2, -MaxLength / 2, 6));
	TopLeftCorner = GetActorLocation() + CornerOffset;

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
		FVector ProductLocation = 10 * GetActorRotation().RotateVector(AvailableSpot) + TopLeftCorner;
		FTransform TempTran = FTransform(GetActorRotation(), ProductLocation, FVector(1,1,1));
		V_LOGM("10x: %s. AS: %s. ActL: %s", *(10 * AvailableSpot).ToString(), *AvailableSpot.ToString(), *GetActorLocation().ToString());
		TempTran.SetRotation(GetActorRotation().Quaternion());

		//DrawDebugSphere(GetWorld(), GetActorRotation().RotateVector(ProductLocation), 8, 16, FColor::Red, true);

		
		/*UChildActorComponent* NewComp = NewObject<UChildActorComponent>(this);
		NewComp->bEditableWhenInherited = true;
		NewComp->RegisterComponent();
		NewComp->SetChildActorClass(AProduct::StaticClass());
		NewComp->CreateChildActor();*/
		
		AProduct* Product = GetWorld()->SpawnActor<AProduct>(AProduct::StaticClass(), TempTran);
		FVector NewLocation = ProductLocation +
			GetActorRotation().RotateVector(FVector(Product->MeshBoxSize.X / 2,
				Product->MeshBoxSize.Y / 2,
				Product->MeshBoxSize.Z / 2));

		//NewLocation = GetActorRotation().RotateVector(NewLocation);

		//DrawDebugSphere(GetWorld(), NewLocation, 8, 16, FColor::Magenta, true);

		Product->SetActorLocation(NewLocation);
		Product->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		
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

void APallet::MovePallet(FVector NewLoc, FRotator NewRotation)
{
	FVector CornerOffset = NewRotation.RotateVector(FVector(-MaxWidth / 2, -MaxLength / 2, 6));
	TopLeftCorner = NewLoc + CornerOffset;

	SetActorLocation(NewLoc);
	SetActorRotation(NewRotation);
}

AProduct* APallet::GetProductFromPallet()
{
	AProduct* PeekProd = StackOfProducts[StackOfProducts.Num()-1];
	FVector SpotOnPallet = GetActorRotation().GetInverse().RotateVector(PeekProd->GetActorLocation() -
			TopLeftCorner - GetActorRotation().RotateVector(PeekProd->MeshBoxSize) / 2)
			/ 10;
	if (SpotOnPallet.X > MaxWidth / 10 || SpotOnPallet.Y > MaxLength / 10 || SpotOnPallet.Z > MaxHeight / 10)
	{
		V_LOGM("Vec: %s", *SpotOnPallet.ToString())
			V_LOGM("ProdLoc %s", *PeekProd->GetActorLocation().ToString());
		return nullptr;
	}

	if (StackOfProducts.Num() > 0) 
	{
		AProduct* Product = StackOfProducts.Pop();

		
		DrawDebugSphere(GetWorld(), SpotOnPallet, 15, 16, FColor::Magenta, true);

		V_LOGM("ALoc: %s, Sp: %s", *Product->GetActorLocation().ToString(), *SpotOnPallet.ToString());
		FVector test = GetActorRotation().RotateVector(-SpotOnPallet);
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
	
	//FVector CornerOffset = GetActorRotation().RotateVector();
	

	FVector Up = FVector(TopLeftCorner.X, TopLeftCorner.Y, 500);
	DEBUGL(TopLeftCorner, Up , .1f);
	DrawDebugBox(GetWorld(), GetActorLocation() + FVector(0, 0, MaxHeight / 2 + 6),
		FVector(MaxWidth / 2, MaxLength / 2, MaxHeight / 2), GetActorRotation().Quaternion(), FColor::Blue, false, 0.1f, (uint8) '\000', 1.f);

}

