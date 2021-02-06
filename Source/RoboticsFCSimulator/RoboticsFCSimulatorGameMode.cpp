// Copyright Epic Games, Inc. All Rights Reserved.

#include "RoboticsFCSimulatorGameMode.h"
#include "RoboticsFCSimulatorPlayerController.h"
#include "RoboticsFCSimulatorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Core/GameStateRoboticsFCSimulator.h"

ARoboticsFCSimulatorGameMode::ARoboticsFCSimulatorGameMode()
{
	// use our custom PlayerController class
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDownCPP/Blueprints/BP_RoboticsFCPlayerController"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/BP_CameraPawn"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateBPClass(TEXT("/Game/Core/BP_GameStateRoboticsFCSimulator"));
	if (GameStateBPClass.Class != nullptr)
	{
		GameStateClass = GameStateBPClass.Class;
	}

	
}