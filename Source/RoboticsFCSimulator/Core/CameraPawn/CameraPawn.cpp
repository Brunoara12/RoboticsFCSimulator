// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraPawn.h"


#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/ArrowComponent.h"
#include "../HelperFiles/DefinedDebugHelpers.h"


// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a Scene Component...
	MovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	//// Create a Scene Component...
	//ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	//RootComponent = ArrowComp;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(ArrowComp);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, -90.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraPawn"));
	PawnCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	PawnCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	

	//RootComponent->SetRelativeRotation(FRotator(0, -90, 0));

	//CameraBoom->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ACameraPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACameraPawn::MoveForward);

}

void ACameraPawn::MoveRight(float AxisValue)
{
	AddMovementInput(FVector(1, 0, 0), AxisValue);
}

void ACameraPawn::MoveForward(float AxisValue)
{
	AddMovementInput(FVector(0, -1, 0), AxisValue);
}


