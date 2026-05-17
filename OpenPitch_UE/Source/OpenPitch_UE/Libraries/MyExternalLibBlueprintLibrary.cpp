#include "MyExternalLibBlueprintLibrary.h"
#include "OpenPitch.h"

FString UMyExternalLibBlueprintLibrary::GetExternalMessage()
{
	OpenPitchMatchResult Result{};

	OpenPitch_RunMatch(&Result);

	return FString::Printf(
		TEXT("Home %d - %d Away"),
		Result.HomeScore,
		Result.AwayScore
	);
}