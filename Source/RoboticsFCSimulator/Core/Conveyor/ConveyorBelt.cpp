// Fill out your copyright notice in the Description page of Project Settings.

#include "ConveyorBelt.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "../HelperFiles/DefinedDebugHelpers.h"

// Sets default values
AConveyorBelt::AConveyorBelt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Core/Conveyor/SM_ConveyorBelt.SM_ConveyorBelt'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>OMeshAsset(TEXT("StaticMesh'/Game/Core/Conveyor/Overlap.Overlap'"));
	//MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	if (MeshAsset.Succeeded() && OMeshAsset.Succeeded())
	{
		Mesh = MeshAsset.Object;
		OverlapMesh = OMeshAsset.Object;
	}

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	RootComponent = SplineComp;
	//MeshComp->AddLocalOffset(FVector(0, 0, -50));
	

	DynamicSpeed = 1;
}

void AConveyorBelt::OnMoveObjects()
{
	MovedActors.Empty();
	MovedActors.AddUnique(this);
	//V_LOG("INMOVEOB");
	for (auto OverlapComp : OverlapComponents)
	{
		TArray<AActor*> OverlappingActors;

		if (OverlapComp != NULL)
		{
			OverlapComp->GetOverlappingActors(OverlappingActors);
			for (auto OverlappingActor : OverlappingActors)
			{
				if (MovedActors.Find(OverlappingActor) == -1 && !OverlappingActor->ActorHasTag("Robot")) {

					FVector DeltaLocation = DynamicSpeed * SplineComp->FindDirectionClosestToWorldLocation(OverlappingActor->GetActorLocation(), ESplineCoordinateSpace::Local);

					DeltaLocation = this->GetActorRotation().RotateVector(DeltaLocation);

					OverlappingActor->AddActorWorldOffset(DeltaLocation);

					MovedActors.Add(OverlappingActor);
				}
			}
		}
	}
}


void AConveyorBelt::HandleUpdate(float Value)
{
	for (auto MeshComp : MeshComponents)
	{
		if (MeshComp)
		{
			V_LOG("Should Move");
			MeshComp->SetScalarParameterValueOnMaterials(FName("PTime"), Value * DynamicSpeed);
		}
	}
}

// Called when the game starts or when spawned
void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(MoveObjectsHandle, this, &AConveyorBelt::OnMoveObjects, 0.01, true);

	if (TimeCurve) 
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleUpdate"));

		MyTimeline.AddInterpFloat(TimeCurve, ProgressFunction);
		MyTimeline.SetLooping(true);

		MyTimeline.PlayFromStart();
	}
}

void AConveyorBelt::OnConstruction(const FTransform& Transform)
{
	int32 SplinePoints = SplineComp->GetNumberOfSplinePoints();
	OverlapComponents.Empty();
	MeshComponents.Empty();
	for (int i = 0; i < SplinePoints - 1; i++)
	{
		FVector SLocation, STangent, ELocation, ETangent;
		SplineComp->GetLocationAndTangentAtSplinePoint(i, SLocation, STangent, ESplineCoordinateSpace::Local);
		SplineComp->GetLocationAndTangentAtSplinePoint(i + 1, ELocation, ETangent, ESplineCoordinateSpace::Local);
		USplineMeshComponent* SMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		USplineMeshComponent* OMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		OverlapComponents.Add(OMesh);
		MeshComponents.Add(SMesh);

		OMesh->SetStaticMesh(OverlapMesh);
		OMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		OMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
		OMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		OMesh->AddLocalOffset(FVector(0, 0, 0));
		OMesh->SetVisibility(false);


		float XScaleValue = (SplineComp->GetDistanceAlongSplineAtSplinePoint(i + 1) -
		SplineComp->GetDistanceAlongSplineAtSplinePoint(i)) / 200;
		SMesh->SetStaticMesh(Mesh);
		SMesh->SetScalarParameterValueOnMaterials(FName("XScale"), XScaleValue);
		SMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		SMesh->SetGenerateOverlapEvents(false);
		SMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SMesh->RegisterComponentWithWorld(GetWorld());
		SMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		OMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SMesh->SetMobility(EComponentMobility::Movable);
		OMesh->SetMobility(EComponentMobility::Movable);

		SMesh->AttachToComponent(SplineComp, FAttachmentTransformRules::KeepRelativeTransform);
		OMesh->AttachToComponent(SplineComp, FAttachmentTransformRules::KeepRelativeTransform);

		SMesh->SetStartAndEnd(SLocation, STangent, ELocation, ETangent);
		OMesh->SetStartAndEnd(SLocation, STangent, ELocation, ETangent);

	}
}

// Called every frame
void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);
}

