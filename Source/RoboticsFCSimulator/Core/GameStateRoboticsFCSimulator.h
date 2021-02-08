// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Interfaces/HUDInterface.h"
#include "../RoboticsFCSimulatorPlayerController.h"
#include "GameStateRoboticsFCSimulator.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTICSFCSIMULATOR_API AGameStateRoboticsFCSimulator : public AGameStateBase, public IHUDInterface
{
	GENERATED_BODY()

public:

	// Seconds it takes for one hour in game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		float TimeUnit= 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
	float NumOfHours;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		int32 Seconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		int32 Minutes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		int32 Hours;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		int32 Day = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		int32 Month = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		int32 Year = 2021;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		TArray<int32> ActualGameTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		TArray<int32> ActualGameDate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time, meta = (AllowPrivateAccess = "true"))
		int32 NumOfDays;
	
	ARoboticsFCSimulatorPlayerController* PlayerC;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void SetTime(const TArray<int32>& Time);
	virtual void SetTime_Implementation(const TArray<int32>& Time);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void SetDate(const TArray<int32>& Date);
	virtual void SetDate_Implementation(const TArray<int32>& Date);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
