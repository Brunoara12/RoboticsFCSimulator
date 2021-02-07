// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"

#include "Components/TimelineComponent.h"

#include "ConveyorBelt.generated.h"

UCLASS()
class ROBOTICSFCSIMULATOR_API AConveyorBelt : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConveyorBelt();

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	class USplineComponent* SplineComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		UStaticMesh* OverlapMesh;
	//UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
	TArray<USplineMeshComponent*> MeshComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
		TArray<USplineMeshComponent*> OverlapComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline")
		TArray<AActor*> MovedActors;

	UPROPERTY(EditAnywhere, Category = "Spline")
	float DynamicSpeed  ;


	// Timer to move Objects
	FTimerHandle MoveObjectsHandle;

	UFUNCTION()
		void OnMoveObjects();

	// Timeline to move Textures
	UPROPERTY(EditAnywhere)
		UCurveFloat* TimeCurve;

	UFUNCTION()
		void HandleUpdate(float Value);
	

	FTimeline MyTimeline;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AActor* GetCloset(FVector start,float maxDisance);

};
