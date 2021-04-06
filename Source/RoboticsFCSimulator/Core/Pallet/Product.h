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

UENUM()
enum class DestCode : uint8
{
	AA,
	BB,
	CC,
	DD,
	EE,
	FF,
	GG,
	HH,
	None
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
	DestCode DestAddress;
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
