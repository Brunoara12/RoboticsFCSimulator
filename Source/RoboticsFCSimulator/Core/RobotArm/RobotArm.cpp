// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotArm.h"


#include "Debugging/SlateDebugging.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Windows/WindowsApplication.h"


// Sets default values
ARobotArm::ARobotArm():input(nullptr),output(nullptr),currentProduct(nullptr),dropPoint(FVector(0,0,0))
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
		currentProduct = input->GetProductFromPallet();
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
	currentProduct->SetActorLocation(FMath::Lerp(currentProduct->GetActorLocation(),dropPoint,0.05));
	if((currentProduct->GetActorLocation() - dropPoint).Size() < 10)
	{
		DropProduct();
		return false;
	}
	
	return true;
}

void ARobotArm::DropProduct()
{
	
	Cast<UStaticMeshComponent>(currentProduct->GetRootComponent())->SetSimulatePhysics(true);
	currentProduct = nullptr;
	
}
bool ARobotArm::InputEmpty()
{
	if(input->NumOfProducts > 0)
	{
		return false;
	}
	return true;
}
//TODO:check if conveyor is full
bool ARobotArm::OutputReady()
{
	return true;
}


void ARobotArm::OnOverlapBegin(UPrimitiveComponent* newComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->ActorHasTag("Pallet") && input == nullptr)
	{
		input = Cast<APallet>(OtherActor);
	}
	else if(OtherActor->ActorHasTag("Conveyor") && output == nullptr)
	{
		output = Cast<AConveyorBelt>(OtherActor);
		dropPoint = output->GetActorLocation() + FVector(0,0,100) + (output->GetActorForwardVector()*30);
	}
}

