// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Product.h"
#include "Pallet.generated.h"


USTRUCT()
struct FPalletRow {
	GENERATED_BODY()

		UPROPERTY()
		TArray<bool> Cols;

	FORCEINLINE bool& operator[] (int32 j)
	{
		return Cols[j];
	}

	void SetState(bool isOccupied, int32 y)
	{
		Cols[y] = isOccupied;
	}

	void AddNewColumn()
	{
		Cols.Add(false);
	}

	FPalletRow()
	{

	}
};

USTRUCT()
struct FPalletLayer {
	GENERATED_BODY()

		UPROPERTY()
		TArray<FPalletRow> Rows;

	bool LayerFull;

	FORCEINLINE FPalletRow& operator[] (int32 i)
	{
		return Rows[i];
	}

	void AddNewRow()
	{
		LayerFull = false;
		Rows.Add(FPalletRow());
	}

	void AddUninitialized(const int32 RowCount, const int32 ColCount)
	{
		Clear();
		//Add Rows
		for (int32 v = 0; v < RowCount; v++)
		{
			AddNewRow();
		}

		//Add Columns
		for (int32 v = 0; v < RowCount; v++)
		{
			for (int32 b = 0; b < ColCount; b++)
			{
				Rows[v].AddNewColumn();
			}
		}
	}

	void Clear()
	{
		if (Rows.Num() <= 0) return;
		//~~~~~~~~~~~~~~~

		//Destroy any Actors
		const int32 RowTotal = Rows.Num();
		const int32 ColTotal = Rows[0].Cols.Num();

		//Empty
		for (int32 v = 0; v < Rows.Num(); v++)
		{
			Rows[v].Cols.Empty();
		}
		Rows.Empty();
	}

	TTuple<int32, int32> NumLayerDimensions()
	{
		return TTuple<int32, int32>(Rows.Num(), Rows[0].Cols.Num());
	}

	//default properties
	FPalletLayer()
	{

	}

	FPalletLayer(int32 Rows, int32 Cols)
	{
		AddUninitialized(Rows, Cols);
	}
};

USTRUCT()
struct FPalletData {
	GENERATED_BODY()

		UPROPERTY()
		TArray<FPalletLayer> Layers;

	FORCEINLINE FPalletLayer& operator[] (int32 i)
	{
		return Layers[i];
	}

	void AddNewLayer()
	{
		Layers.Add(FPalletLayer());
	}

	void AddUnitialized(int32 LayerCount, int32 RowCount, int32 ColCount)
	{
		Clear();
		// Add Layers
		for (int32 l = 0; l < LayerCount; l++)
		{
			AddNewLayer();
		}

		// Add Rows
		for (int32 l = 0; l < LayerCount; l++)
		{
			for (int32 r = 0; r < RowCount; r++)
			{
				Layers[l].AddNewRow();
			}
		}

		// Add Rows
		for (int32 l = 0; l < LayerCount; l++)
		{
			for (int32 r = 0; r < RowCount; r++)
			{
				for (int32 c = 0; c < ColCount; c++)
				{
					Layers[l].Rows[r].AddNewColumn();
				}
			}
		}
	}

	void Clear()
	{
		if (Layers.Num() <= 0) return;
		//~~~~~~~~~~~~~~~

		//Destroy any Actors
		const int32 LayerTotal = Layers.Num();
		const int32 RowTotal = Layers[0].Rows.Num();
		const int32 ColTotal = Layers[0].Rows[0].Cols.Num();


		//Empty
		for (int32 l = 0; l < LayerTotal; l++)
		{
			for (int32 r = 0; r < RowTotal; r++)
			{
				Layers[l].Rows[r].Cols.Empty();
			}
		}
		for (int32 l = 0; l < LayerTotal; l++)
		{
			Layers[l].Rows.Empty();
		}

		Layers.Empty();
	}

	bool CheckIfOccupied(FVector Coord)
	{
		return Layers[Coord.Z][Coord.Y][Coord.X];
	}

	FVector GetNextAvailableSpot()
	{
		for (int z = 0; z < Layers.Num(); z++)
		{
			if (Layers[z].LayerFull == false)
			{
				for (int y = 0; y < Layers[0].Rows.Num(); y++)
				{
					for (int x = 0; x < Layers[0].Rows[0].Cols.Num(); x++)
					{
						if (Layers[z][y][x] == false)
						{
							return FVector(x, y, z);
						}
					}
				}
			}
		}

		return FVector(-1, -1, -1);
	}

	void SetBox(FVector Start, FVector Extent, bool bOccupied)
	{
		for (int z = Start.Z; z <= Extent.Z; z++)
		{
			for (int y = Start.Y; y <= Extent.Y; y++)
			{
				for (int x = Start.X; x <= Extent.X; x++)
				{
					Layers[z][y][x] = bOccupied;
				}
			}
		}
	}

	FPalletData()
	{
		AddUnitialized(3, 3, 3);
	}

	FPalletData(int32 size)
	{
		AddUnitialized(size, size, size);
	}

	FPalletData(int32 Layers, int32 Rows, int32 Cols)
	{
		AddUnitialized(Layers, Rows, Cols);
	}
	
};

UCLASS()
class ROBOTICSFCSIMULATOR_API APallet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APallet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pallet, meta = (AllowPrivateAccess = "true"))
	int32 NumOfProductsToFillPallet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pallet, meta = (AllowPrivateAccess = "true"))
		int32 NumOfProducts;

	int32 MaxHeight;
	int32 MaxWidth;
	int32 MaxLength;

	UPROPERTY()
	FPalletData Pallet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pallet, meta = (AllowPrivateAccess = "true"))
	FVector TopLeftCorner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pallet, meta = (AllowPrivateAccess = "true"))
		TArray<AProduct *> StackOfProducts;

	TArray<UChildActorComponent*> ProductsInPallet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;

	virtual void OnConstruction(const FTransform& Transform) override;

	void ConstructFullPallet();
	
	UFUNCTION(BlueprintCallable)
	void AddProductToPallet();

	UFUNCTION(BlueprintCallable)
		void RemoveProductFromPallet();
	
	UFUNCTION(BlueprintCallable)
		void MovePallet(FVector NewLoc, FRotator NewRotation);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//returns product removed and removes product association with pallet
	UFUNCTION(BlueprintCallable)
    AProduct* GetProductFromPallet();

	FVector GetNextAvailiblePosition();
	bool ReadyForNewProduct();
	//adds product to palletData but does not spawn a new product
	void StackProductOnPallet(AProduct* Product,FVector Location);

};
