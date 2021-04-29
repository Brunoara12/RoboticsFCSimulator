// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Product.generated.h"

UENUM()
enum class ShippingType : uint8
{
	Nextday,
	NoRush,
	Standard
};

UENUM(BlueprintType)
enum class EDestCode : uint8
{
	AA UMETA(DisplayName = "AA"), 
	BB UMETA(DisplayName = "BB"),
	CC UMETA(DisplayName = "CC"),
	DD UMETA(DisplayName = "DD"),
	EE UMETA(DisplayName = "EE"),
	FF UMETA(DisplayName = "FF"),
	GG UMETA(DisplayName = "GG"),
	HH UMETA(DisplayName = "HH"),
	None UMETA(DisplayName = "None")
};

UCLASS()
class ROBOTICSFCSIMULATOR_API AProduct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProduct();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RetAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDestCode DestAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ShippingType shippingType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString NameOfProduct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pallet, meta = (AllowPrivateAccess = "true"))
	FVector MeshBoxSize;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComp;

	UMaterialInterface* MaterialInt;

//	UMaterialInstanceDynamic* Material_Dyn;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
