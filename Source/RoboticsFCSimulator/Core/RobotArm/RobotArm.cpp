// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotArm.h"


#include "Debugging/SlateDebugging.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Windows/WindowsApplication.h"


// Sets default values
ARobotArm::ARobotArm():input(nullptr),output(nullptr),currentProduct(nullptr),dropPoint(FVector(0,0,0)),deltaTime(0)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//CreateDefaultSubobject<USphereComponent>(TEXT("This thing"));
	SphereCom = CreateDefaultSubobject<USphereComponent>(TEXT("InputSetter"));
	SphereCom->InitSphereRadius(20.0);
	RootComponent = SphereCom;
}

// Called when the game starts or when spawned
void ARobotArm::BeginPlay()
{
	Super::BeginPlay();
	
	Cast<USphereComponent>(GetRootComponent())->OnComponentBeginOverlap.AddDynamic(this, &ARobotArm::OnOverlapBegin);
	
}

// Called every frame
void ARobotArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->deltaTime = DeltaTime;



}

bool ARobotArm::CheckDistanceToInputs()
{
	if((GetActorLocation()- input->GetRootComponent()->GetComponentLocation() ).Size() > 250 && !(input->ActorHasTag("Conveyor")))
	{
		input = nullptr;
		return false;
	}
	return true;
}
bool ARobotArm::SetupInputs()
{
	if ((input == nullptr || output == nullptr) && SphereCom->GetScaledSphereRadius() < 250.0f)
	{
		SphereCom->SetSphereRadius(SphereCom->GetScaledSphereRadius() +10);
		return false;
	}
	if((input != nullptr || output != nullptr) && SphereCom->GetScaledSphereRadius() >= 250.0f)
	{
		SphereCom->SetSphereRadius(10);
		return false;
	}
	if ((input != nullptr && output != nullptr))
	{
		SphereCom->SetSphereRadius(10); 
		return true;
	}
	return true;
	
}
// Called to bind functionality to input
void ARobotArm::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ARobotArm::PickupProduct()
{
	if(!InputEmpty() && OutputReady())
	{
		if(input->ActorHasTag("Pallet"))
		{
			currentProduct = Cast<APallet>(input)->GetProductFromPallet();
		}
		if(input->ActorHasTag("Conveyor"))
		{
			
			currentProduct = Cast<AProduct>(Cast<AConveyorBelt>(input)->GetCloset(RootComponent->GetComponentLocation(),200));
			if (currentProduct == nullptr)
			{
				return false;
			}
		}
		
		Cast<UStaticMeshComponent>(currentProduct->GetRootComponent())->SetSimulatePhysics(false);
		return true;
	}
	return false;
}

bool ARobotArm::Transfer()
{
	if(currentProduct == nullptr)
	{
		PickupProduct();
		return true;
		
	}
	currentProduct->SetActorLocation(FMath::Lerp(currentProduct->GetActorLocation(),dropPoint,5* deltaTime));
	if((currentProduct->GetActorLocation() - dropPoint).Size() <= .50)
	{
		DropProduct();
		return false;
	}
	
	return true;
}

void ARobotArm::DropProduct()
{
	if(output->ActorHasTag("Conveyor"))
		Cast<UStaticMeshComponent>(currentProduct->GetRootComponent())->SetSimulatePhysics(true);
	else if(output->ActorHasTag("Pallet"))
		Cast<APallet>(output)->StackProductOnPallet(currentProduct,dropPoint);
	currentProduct = nullptr;
	
}
bool ARobotArm::InputEmpty()
{
	if(input->ActorHasTag("Pallet") && Cast<APallet>(input)->NumOfProducts > 0)
	{
		return false;
	}
	if(input->ActorHasTag("Conveyor") && Cast<AConveyorBelt>(input)->MovedActors.Num() > 0 && Cast<AProduct>(Cast<AConveyorBelt>(input)->GetCloset(RootComponent->GetComponentLocation(),200)) != nullptr)
	{
		return false;
	}
	return true;
}
//TODO:check if conveyor is full
bool ARobotArm::OutputReady()
{
	if(output->ActorHasTag("Pallet") && Cast<APallet>(output)->ReadyForNewProduct())
	{
		return true;
	}
	return true;
}


void ARobotArm::OnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Pallet") || OtherActor->ActorHasTag("Conveyor"))
	{
		FVector dir = OtherActor->GetActorLocation() - RootComponent->GetComponentLocation();
		if(dir.Y > 0   && input == nullptr)
		{
			input = OtherActor;
		}
		else if(dir.Y < 0 && output == nullptr)
		{
			output = OtherActor;
		
		}
	}
}

void ARobotArm::GetDropPoint()
{

	if(output->ActorHasTag("Conveyor"))
		dropPoint = output->GetActorLocation() + FVector(0,0,50) + (output->GetActorForwardVector()*30);
	else
	{
		dropPoint = Cast<APallet>(output)->GetNextAvailiblePosition() + FVector(currentProduct->MeshBoxSize.X / 2,
                    currentProduct->MeshBoxSize.Y / 2,
                    currentProduct->MeshBoxSize.Z / 2);
	}
}

void ARobotArm::ClearInputs()
{
	input = nullptr;
	output = nullptr;
	SphereCom->SetSphereRadius(10);

}