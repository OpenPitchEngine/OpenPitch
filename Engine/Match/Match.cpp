#include "Match.h"

#include <algorithm>
#include <random>

extern "C"
void OpenPitch_RunMatch(
    OpenPitch::OpenPitchMatchResult* outResult
)
{
    using namespace OpenPitch;

    Team homeTeam{
        "Home",
        {
            {"Player 1"},
            {"Player 2"},
            {"Player 3"},
            {"Player 4"},
            {"Player 5"},
            {"Player 6"},
            {"Player 7"},
            {"Player 8"},
            {"Player 9"},
            {"Player 10"},
            {"Player 11"}
        },
        7,
        6,
        5
    };

    Team awayTeam{
        "Away",
        {
            {"Player A"},
            {"Player B"},
            {"Player C"},
            {"Player D"},
            {"Player E"},
            {"Player F"},
            {"Player G"},
            {"Player H"},
            {"Player I"},
            {"Player J"},
            {"Player K"}
        },
        6,
        7,
        6
    };

    Match match(
        homeTeam,
        awayTeam
    );

    const Result result =
        match.Play();

    outResult->HomeScore =
        result.HomeScore;

    outResult->AwayScore =
        result.AwayScore;

    outResult->HomePossession =
        result.HomePossession;

    outResult->AwayPossession =
        result.AwayPossession;

    outResult->HomeShots =
        result.HomeShots;

    outResult->AwayShots =
        result.AwayShots;

    outResult->HomeShotsOnTarget =
        result.HomeShotsOnTarget;

    outResult->AwayShotsOnTarget =
        result.AwayShotsOnTarget;
}

namespace OpenPitch
{
    Match::Match(
        const Team& homeTeam,
        const Team& awayTeam,
        unsigned int seed
    )
        : HomeTeam_(homeTeam),
          AwayTeam_(awayTeam),
          Seed_(seed)
    {
    }

    Result Match::Play()
    {
        Result result;

        result.HomeTeam = HomeTeam_;
        result.AwayTeam = AwayTeam_;

        constexpr int tickCount = 270;

        std::mt19937 generator(
            Seed_ == 0
            ? static_cast<unsigned int>(
                std::random_device{}()
              )
            : Seed_
        );

        // REMOVED: horizontalMovement

        std::uniform_real_distribution<float>
            verticalMovement(-2.0f, 2.0f);

        // REMOVED: turnoverChance

        std::uniform_int_distribution<int>
            passChance(0, 99);

        std::uniform_int_distribution<int>
            shotChance(0, 99);
        std::uniform_int_distribution<int>
            finishChance(0, 99);

        MatchState state;

        state.BallPosition = {
            52.5f,
            34.0f
        };

        state.HomePossession = true;

        state.PossessingPlayer = 0;

        for (size_t i = 0;
             i < HomeTeam_.Players.size();
             ++i)
        {
            PlayerState player;

            player.BasePosition = {
                20.0f + static_cast<float>(i * 2),
                10.0f + static_cast<float>(i * 4)
            };

            player.Position =
                player.BasePosition;

            state.HomePlayers.push_back(player);
        }

        for (size_t i = 0;
             i < AwayTeam_.Players.size();
             ++i)
        {
            PlayerState player;

            player.BasePosition = {
                85.0f - static_cast<float>(i * 2),
                10.0f + static_cast<float>(i * 4)
            };

            player.Position =
                player.BasePosition;

            state.AwayPlayers.push_back(player);
        }

        if (!state.HomePlayers.empty())
        {
            state.HomePlayers[0].HasBall = true;
        }

        bool goalScoredThisTick = false;

        int homePossessionTicks = 0;
        int awayPossessionTicks = 0;

        for (int tick = 0; tick < tickCount; ++tick)
        {
            goalScoredThisTick = false;

            const int currentMinute =
                static_cast<int>(
                    (
                        static_cast<float>(tick)
                        / static_cast<float>(tickCount)
                    ) * 90.0f
                );

            // --- Begin new player movement and turnover logic ---
            if (state.HomePossession)
            {
                homePossessionTicks++;
            }
            else
            {
                awayPossessionTicks++;
            }

            std::vector<PlayerState>& attackingPlayers =
                state.HomePossession
                ? state.HomePlayers
                : state.AwayPlayers;

            std::vector<PlayerState>& defendingPlayers =
                state.HomePossession
                ? state.AwayPlayers
                : state.HomePlayers;

            PlayerState& carrier =
                attackingPlayers[
                    state.PossessingPlayer
                ];

            carrier.HasBall = true;

            const float attackDirection =
                state.HomePossession
                ? 1.0f
                : -1.0f;

            for (size_t i = 0;
                 i < attackingPlayers.size();
                 ++i)
            {
                auto& player = attackingPlayers[i];

                player.Position.X +=
                    (
                        player.BasePosition.X
                        - player.Position.X
                    ) * 0.03f;

                player.Position.Y +=
                    (
                        player.BasePosition.Y
                        - player.Position.Y
                    ) * 0.03f;

                if (&player != &carrier)
                {
                    const float supportOffset =
                        static_cast<float>(i) * 0.35f;

                    player.Position.X +=
                        attackDirection
                        * (0.35f + supportOffset);
                }
            }

            for (auto& player : defendingPlayers)
            {
                player.Position.X +=
                    (
                        carrier.Position.X
                        - player.Position.X
                    ) * 0.04f;

                player.Position.Y +=
                    (
                        carrier.Position.Y
                        - player.Position.Y
                    ) * 0.04f;
            }

            carrier.Position.X +=
                attackDirection
                * 1.8f;

            carrier.Position.Y +=
                verticalMovement(generator)
                * 1.2f;

            state.BallPosition =
                carrier.Position;

            bool turnover = false;

            for (const auto& defender : defendingPlayers)
            {
                const float dx =
                    defender.Position.X
                    - carrier.Position.X;

                const float dy =
                    defender.Position.Y
                    - carrier.Position.Y;

                const float distanceSquared =
                    (dx * dx)
                    + (dy * dy);

                if (distanceSquared < 4.0f)
                {
                    turnover = true;
                    break;
                }
            }

            if (turnover)
            {
                carrier.HasBall = false;

                state.HomePossession =
                    !state.HomePossession;

                state.PossessingPlayer = 0;
            }
            // --- End new player movement and turnover logic ---

            if (passChance(generator) < 55)
            {
                int bestTarget =
                    state.PossessingPlayer;

                float furthestProgress =
                    carrier.Position.X * attackDirection;

                for (size_t i = 0;
                     i < attackingPlayers.size();
                     ++i)
                {
                    if (
                        static_cast<int>(i)
                        == state.PossessingPlayer
                    )
                    {
                        continue;
                    }

                    const auto& teammate =
                        attackingPlayers[i];

                    const float progress =
                        teammate.Position.X
                        * attackDirection;

                    if (progress > furthestProgress)
                    {
                        furthestProgress = progress;
                        bestTarget = static_cast<int>(i);
                    }
                }

                if (bestTarget != state.PossessingPlayer)
                {
                    carrier.HasBall = false;

                    state.PossessingPlayer =
                        bestTarget;

                    PlayerState& targetPlayer =
                        attackingPlayers[
                            state.PossessingPlayer
                        ];

                    targetPlayer.HasBall = true;

                    targetPlayer.Position.X +=
                        attackDirection * 1.2f;

                    state.BallPosition =
                        targetPlayer.Position;
                }
            }

            const bool inShootingRange =
                state.HomePossession
                ? carrier.Position.X >= 72.0f
                : carrier.Position.X <= 33.0f;

            if (
                inShootingRange
                && shotChance(generator) < 12
            )
            {
                if (state.HomePossession)
                {
                    result.HomeShots++;

                    result.Events.push_back({
                        currentMinute,
                        "Home attempt on goal"
                    });

                    const bool onTarget =
                        shotChance(generator) < 65;

                    if (onTarget)
                    {
                        result.HomeShotsOnTarget++;

                        result.Events.push_back({
                            currentMinute,
                            "Home shot on target"
                        });

                        const bool scored =
                            finishChance(generator) < 38;

                        if (scored)
                        {
                            state.ShotInProgress = true;
                            state.ShotByHomeTeam = true;

                            state.BallPosition.X = 104.0f;
                        }
                        else
                        {
                            state.HomePossession = false;
                            state.PossessingPlayer = 0;

                            state.BallPosition.X = 94.0f;
                        }
                    }
                    else
                    {
                        state.BallPosition.X = 92.0f;

                        result.Events.push_back({
                            currentMinute,
                            "Home misses the target"
                        });
                    }
                }
                else
                {
                    result.AwayShots++;

                    result.Events.push_back({
                        currentMinute,
                        "Away attempt on goal"
                    });

                    const bool onTarget =
                        shotChance(generator) < 65;

                    if (onTarget)
                    {
                        result.AwayShotsOnTarget++;

                        result.Events.push_back({
                            currentMinute,
                            "Away shot on target"
                        });

                        const bool scored =
                            finishChance(generator) < 38;

                        if (scored)
                        {
                            state.ShotInProgress = true;
                            state.ShotByHomeTeam = false;

                            state.BallPosition.X = 1.0f;
                        }
                        else
                        {
                            state.HomePossession = true;
                            state.PossessingPlayer = 0;

                            state.BallPosition.X = 11.0f;
                        }
                    }
                    else
                    {
                        state.BallPosition.X = 13.0f;

                        result.Events.push_back({
                            currentMinute,
                            "Away misses the target"
                        });
                    }
                }
            }

            if (!goalScoredThisTick && state.ShotInProgress)
            {
                const bool homeGoalScored =
                    state.ShotByHomeTeam
                    && state.BallPosition.X >= 104.0f;

                const bool awayGoalScored =
                    !state.ShotByHomeTeam
                    && state.BallPosition.X <= 1.0f;

                if (homeGoalScored)
                {
                    result.HomeScore++;

                    result.Events.push_back({
                        currentMinute,
                        "GOAL: Home scores"
                    });

                    state.ShotInProgress = false;

                    state.BallPosition.X = 52.5f;
                    state.BallPosition.Y = 34.0f;

                    state.HomePossession = false;
                    state.PossessingPlayer = 0;

                    for (auto& player : state.HomePlayers)
                    {
                        player.HasBall = false;
                        player.Position = player.BasePosition;
                    }

                    for (auto& player : state.AwayPlayers)
                    {
                        player.HasBall = false;
                        player.Position = player.BasePosition;
                    }

                    state.AwayPlayers[0].HasBall = true;

                    goalScoredThisTick = true;
                }
                else if (awayGoalScored)
                {
                    result.AwayScore++;

                    result.Events.push_back({
                        currentMinute,
                        "GOAL: Away scores"
                    });

                    state.ShotInProgress = false;

                    state.BallPosition.X = 52.5f;
                    state.BallPosition.Y = 34.0f;

                    state.HomePossession = true;
                    state.PossessingPlayer = 0;

                    for (auto& player : state.HomePlayers)
                    {
                        player.HasBall = false;
                        player.Position = player.BasePosition;
                    }

                    for (auto& player : state.AwayPlayers)
                    {
                        player.HasBall = false;
                        player.Position = player.BasePosition;
                    }

                    state.HomePlayers[0].HasBall = true;

                    goalScoredThisTick = true;
                }
            }

            state.BallPosition =
                carrier.Position;

            state.BallPosition.X = std::clamp(
                state.BallPosition.X,
                1.0f,
                104.0f
            );

            state.BallPosition.Y = std::clamp(
                state.BallPosition.Y,
                0.0f,
                68.0f
            );

            const int totalPossessionTicks =
                homePossessionTicks
                + awayPossessionTicks;

            if (totalPossessionTicks > 0)
            {
                result.HomePossession =
                    static_cast<int>(
                        (
                            static_cast<float>(
                                homePossessionTicks
                            )
                            / static_cast<float>(
                                totalPossessionTicks
                            )
                        ) * 100.0f
                    );

                result.AwayPossession =
                    100 - result.HomePossession;
            }

            Result::Snapshot snapshot;

            snapshot.Minute =
                static_cast<float>(currentMinute);

            snapshot.BallPosition =
                state.BallPosition;

            snapshot.HomeTeamInPossession =
                state.HomePossession;

            snapshot.HomePlayers =
                state.HomePlayers;

            snapshot.AwayPlayers =
                state.AwayPlayers;

            snapshot.HomeScore =
                result.HomeScore;

            snapshot.AwayScore =
                result.AwayScore;

            snapshot.HomePossession =
                result.HomePossession;

            snapshot.AwayPossession =
                result.AwayPossession;

            snapshot.HomeShots =
                result.HomeShots;

            snapshot.AwayShots =
                result.AwayShots;

            snapshot.HomeShotsOnTarget =
                result.HomeShotsOnTarget;

            snapshot.AwayShotsOnTarget =
                result.AwayShotsOnTarget;

            result.Timeline.push_back(snapshot);
        }

        return result;
    }
}