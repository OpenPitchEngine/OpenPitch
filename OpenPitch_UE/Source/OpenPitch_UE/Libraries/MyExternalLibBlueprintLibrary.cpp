#include "MyExternalLibBlueprintLibrary.h"

#include "OpenPitch.h"

FOpenPitchMatchResult
UMyExternalLibBlueprintLibrary::RunMatch()
{
	OpenPitchMatchResult NativeResult{};

	OpenPitch_RunMatch(
		&NativeResult
	);

	FOpenPitchMatchResult Result;

	Result.HomeScore =
		NativeResult.HomeScore;

	Result.AwayScore =
		NativeResult.AwayScore;

	Result.HomePossession =
		NativeResult.HomePossession;

	Result.AwayPossession =
		NativeResult.AwayPossession;

	Result.HomeShots =
		NativeResult.HomeShots;

	Result.AwayShots =
		NativeResult.AwayShots;

	Result.HomeShotsOnTarget =
		NativeResult.HomeShotsOnTarget;

	Result.AwayShotsOnTarget =
		NativeResult.AwayShotsOnTarget;

	return Result;
}