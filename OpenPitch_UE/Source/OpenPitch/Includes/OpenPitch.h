#pragma once

#ifdef _WIN32
    #ifdef OPENPITCH_EXPORTS
        #define OPENPITCH_API __declspec(dllexport)
    #else
        #define OPENPITCH_API __declspec(dllimport)
    #endif
#else
    #define OPENPITCH_API __attribute__((visibility("default")))
#endif

extern "C"
{
struct OpenPitchMatchResult
{
    int HomeScore;
    int AwayScore;

    int HomePossession;
    int AwayPossession;

    int HomeShots;
    int AwayShots;

    int HomeShotsOnTarget;
    int AwayShotsOnTarget;
};

OPENPITCH_API void OpenPitch_RunMatch(
    OpenPitchMatchResult* outResult
);
}