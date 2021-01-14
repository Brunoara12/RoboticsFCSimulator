// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoboticsFCSimulatorGameMode.h"
#include "RoboticsFCSimulatorPlayerController.h"
#include "RoboticsFCSimulatorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARoboticsFCSimulatorGameMode::ARoboticsFCSimulatorGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARoboticsFCSimulatorPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/BP_CameraPawn"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}