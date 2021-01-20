// Fill out your copyright notice in the Description page of Project Settings.


#include "Product.h"

#include "UObject/ConstructorHelpers.h"
#include "../HelperFiles/DefinedDebugHelpers.h"

// Sets default values
AProduct::AProduct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

	if (MeshAsset.Succeeded())
	{
		MeshComp->SetStaticMesh(MeshAsset.Object);
	}

	MeshComp->SetRelativeScale3D(FVector(.5, .6, .3));
	MeshBoxSize = FVector(50, 60, 30);

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Core/Pallet/M_Product.M_Product'"));
	if (Material.Object != NULL)
	{
		MaterialInt = (UMaterial*)Material.Object;
	}
	
	//Material_Dyn = UMaterialInstanceDynamic::Create(MaterialInt, MeshComp);


	Weight = 10;
	NameOfProduct = "Glasses";
}

// Called when the game starts or when spawned
void AProduct::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->SetMaterial(0, MaterialInt);
	UMaterialInstanceDynamic* Material_Dyn = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
	FLinearColor RandomColor;
	RandomColor.R = FMath::RandRange(0, 1);
	RandomColor.G = FMath::RandRange(0, 1);
	RandomColor.B = FMath::RandRange(0, 1);
	RandomColor.A = 1;

	if (Material_Dyn)
	{
		Material_Dyn->SetVectorParameterValue(FName("ProductColor"), RandomColor);
	}
}

// Called every frame
void AProduct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

