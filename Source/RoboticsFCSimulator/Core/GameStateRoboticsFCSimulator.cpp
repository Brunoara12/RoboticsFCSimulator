// Fill out your copyright notice in the Description page of Project Settings.

#include "GameStateRoboticsFCSimulator.h"
#include "Interfaces/HUDInterface.h"
#include "Blueprint/UserWidget.h"
#include "HelperFiles/DefinedDebugHelpers.h"



void AGameStateRoboticsFCSimulator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float SumTimeUnitEachFrame = (DeltaTime / TimeUnit) + NumOfHours;

	NumOfDays = SumTimeUnitEachFrame / 24;
	NumOfHours = FGenericPlatformMath::Fmod(SumTimeUnitEachFrame, 24.f);

	// Day - Hour , Min, Sec
	Seconds = FMath::Floor(FGenericPlatformMath::Fmod(NumOfHours * 3600 , 60));
	float ToMin = (float)NumOfHours * 3600 / 60;

	Minutes = FMath::Floor(FGenericPlatformMath::Fmod(ToMin, 60));
	float ToHour = (float)ToMin / 60;

	Hours = FMath::Floor(FGenericPlatformMath::Fmod(ToHour, 24));

	ActualGameTime.Empty();
	ActualGameTime.Add(Hours);
	ActualGameTime.Add(Minutes);
	ActualGameTime.Add(Seconds);
	int32 test = ActualGameTime[0];


	if (PlayerC->MainHUDPtr->GetClass()->ImplementsInterface(UHUDInterface::StaticClass()))
	{
		Execute_SetTime(Cast<UObject>(PlayerC->MainHUDPtr), ActualGameTime);
	}
	
	

	FString GameTimeString = FString::FromInt(ActualGameTime[0]) + ":" + FString::FromInt(ActualGameTime[1]);

	//SCREENMSG(GameTimeString);

	// Date - Year , Month, Day

	Day = NumOfDays + Day;

	if (Day > FDateTime::DaysInMonth(Year, Month))
	{
		Day = 1;
		Month++;
	}
	
	if (Month > 12)
	{
		Month = 1;
		Year++;
	}

	ActualGameDate.Empty();
	ActualGameDate.Add(Day);
	ActualGameDate.Add(Month);
	ActualGameDate.Add(Year);

	if (PlayerC->MainHUDPtr->GetClass()->ImplementsInterface(UHUDInterface::StaticClass()))
	{
		Execute_SetDate(Cast<UObject>(PlayerC->MainHUDPtr), ActualGameDate);
	}

	FString GameDateString = FString::FromInt(ActualGameDate[1]) + "/" + FString::FromInt(ActualGameDate[0]) + "/" + FString::FromInt(ActualGameDate[2]);
	//SCREENMSGC(GameDateString, Red);

}

void AGameStateRoboticsFCSimulator::SetTime_Implementation(const TArray<int32>& Time)
{

}

void AGameStateRoboticsFCSimulator::SetDate_Implementation(const TArray<int32>& Date)
{

}

void AGameStateRoboticsFCSimulator::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		PlayerC = Cast<ARoboticsFCSimulatorPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	}
}

