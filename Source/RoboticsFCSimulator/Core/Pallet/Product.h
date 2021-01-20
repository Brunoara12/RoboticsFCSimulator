// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Product.generated.h"

UCLASS()
class ROBOTICSFCSIMULATOR_API AProduct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProduct();

	int32 Weight;
	FString NameOfProduct;
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
