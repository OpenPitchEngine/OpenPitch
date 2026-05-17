// MyExternalLibBlueprintLibrary.h

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "MyExternalLibBlueprintLibrary.generated.h"

USTRUCT(BlueprintType)
struct FOpenPitchMatchResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 HomeScore = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 AwayScore = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 HomePossession = 50;

	UPROPERTY(BlueprintReadOnly)
	int32 AwayPossession = 50;

	UPROPERTY(BlueprintReadOnly)
	int32 HomeShots = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 AwayShots = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 HomeShotsOnTarget = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 AwayShotsOnTarget = 0;
};

UCLASS()
class OPENPITCH_UE_API
UMyExternalLibBlueprintLibrary
	: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static FOpenPitchMatchResult RunMatch();
};