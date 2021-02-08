// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "MonthNameEnum.generated.h"

UENUM(BlueprintType)
enum class EMonthNameEnum : uint8
{
	January	UMETA(DisplayName = "January"),
	February UMETA(DisplayName = "February"),
	March	UMETA(DisplayName = "March"),
	April	UMETA(DisplayName = "April"),
	May		UMETA(DisplayName = "May"),
	June	UMETA(DisplayName = "June"),
	July	UMETA(DisplayName = "July"),
	August	UMETA(DisplayName = "August"),
	September UMETA(DisplayName = "September"),
	October	UMETA(DisplayName = "October"),
	November UMETA(DisplayName = "November"),
	December UMETA(DisplayName = "December"),
};

/**
 * 
 */
UCLASS()
class ROBOTICSFCSIMULATOR_API UMonthNameEnum : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
