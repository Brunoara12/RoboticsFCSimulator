// Fill out your copyright notice in the Description page of Project Settings.


#include "Product.h"

#include "UObject/ConstructorHelpers.h"
#include "../HelperFiles/DefinedDebugHelpers.h"

// Sets default values
AProduct::AProduct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
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

	PrimaryActorTick.bCanEverTick = true;
	shippingType = ShippingType::Standard;
	Weight = rand()%20;
	RetAddress = "Lorem Ipsum";
	DestAddress = StaticCast<DestCode>(rand() % 8);
	NameOfProduct = "Glasses";

	MeshComp->SetMaterial(0, MaterialInt);
    	UMaterialInstanceDynamic* Material_Dyn = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
    	 FLinearColor RandomColor;
    
    	switch (DestAddress)
    	{
    		case DestCode::AA:
    			RandomColor.R = 1;
    			RandomColor.G = 0;
    			RandomColor.B = 0;
    			break;
    		case DestCode::BB:
    			RandomColor.R = 0;
    			RandomColor.G = 1;
    			RandomColor.B = 0;
    			break;
    			
    		case DestCode::CC:
    			RandomColor.R = 0;
    			RandomColor.G = 0;
    			RandomColor.B = 1;
    			break;
    		case DestCode::DD:
    			RandomColor.R = 1;
    			RandomColor.G = 1;
    			RandomColor.B = 0;
    			break;
    		case DestCode::EE:
    			RandomColor.R = 1;
    			RandomColor.G = 0;
    			RandomColor.B = 1;
    			break;
    		case DestCode::FF:
    			RandomColor.R = 0;
    			RandomColor.G = 1;
    			RandomColor.B = 1;
    			break;
    		case DestCode::GG:
    			RandomColor.R = 1;
    			RandomColor.G = .2;
    			RandomColor.B = .2;
    			break;
    		case DestCode::HH:
    			RandomColor.R = 1;
    			RandomColor.G = 1;
    			RandomColor.B = 1;
    			break;
    		default:
    			RandomColor.R = 0;
    			RandomColor.G = 0;
    			RandomColor.B = 0;
    			break;
    	}
    
    	 RandomColor.A = 1;
    	 
    	 if (Material_Dyn)
    	 {
    	 	Material_Dyn->SetVectorParameterValue(FName("ProductColor"), RandomColor);
    	 }
}

// Called when the game starts or when spawned
void AProduct::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AProduct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

