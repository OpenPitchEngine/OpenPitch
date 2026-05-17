#pragma once

#include <string>
#include <vector>

namespace OpenPitch
{
    struct Vector2
    {
        float X = 0.0f;
        float Y = 0.0f;
    };

    struct Player
    {
        std::string Name;
    };

    struct PlayerState
    {
        Vector2 Position;

        float Stamina = 100.0f;

        bool HasBall = false;
    };

    struct Team
    {
        std::string Name;

        std::vector<Player> Players;

        int Attack;
        int Midfield;
        int Defence;
    };

    struct Event
    {
        int Minute;

        std::string Description;
    };

    struct MatchState
    {
        Vector2 BallPosition;

        bool HomePossession = true;

        int PossessingPlayer = 0;

        std::vector<PlayerState> HomePlayers;
        std::vector<PlayerState> AwayPlayers;
    };

    struct Result
    {
        Team HomeTeam;
        Team AwayTeam;

        int HomeScore = 0;
        int AwayScore = 0;

        int HomePossession = 50;
        int AwayPossession = 50;

        int HomeShots = 0;
        int AwayShots = 0;

        int HomeShotsOnTarget = 0;
        int AwayShotsOnTarget = 0;

        struct Snapshot
        {
            float Minute = 0.0f;

            Vector2 BallPosition;

            bool HomeTeamInPossession = true;

            std::vector<PlayerState> HomePlayers;
            std::vector<PlayerState> AwayPlayers;

            int HomeScore = 0;
            int AwayScore = 0;

            int HomePossession = 50;
            int AwayPossession = 50;

            int HomeShots = 0;
            int AwayShots = 0;

            int HomeShotsOnTarget = 0;
            int AwayShotsOnTarget = 0;
        };

        std::vector<Snapshot> Timeline;

        std::vector<Event> Events;
    };

    class Match
    {
    public:
        Match(
            const Team& homeTeam,
            const Team& awayTeam,
            unsigned int seed = 0
        );

        Result Play();

    private:
        Team HomeTeam_;
        Team AwayTeam_;

        unsigned int Seed_;
    };
}