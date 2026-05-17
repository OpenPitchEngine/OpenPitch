#include "Match.h"

#include <algorithm>
#include <random>

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

        std::uniform_real_distribution<float>
            horizontalMovement(-3.0f, 3.0f);

        std::uniform_real_distribution<float>
            verticalMovement(-2.0f, 2.0f);

        std::uniform_int_distribution<int>
            turnoverChance(0, 99);

        std::uniform_int_distribution<int>
            passChance(0, 99);

        std::uniform_int_distribution<int>
            shotChance(0, 99);

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

            player.Position = {
                20.0f + static_cast<float>(i),
                10.0f + static_cast<float>(i * 4)
            };

            state.HomePlayers.push_back(player);
        }

        for (size_t i = 0;
             i < AwayTeam_.Players.size();
             ++i)
        {
            PlayerState player;

            player.Position = {
                85.0f - static_cast<float>(i),
                10.0f + static_cast<float>(i * 4)
            };

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

            if (turnoverChance(generator) < 10)
            {
                state.HomePossession = !state.HomePossession;

                result.Events.push_back({
                    currentMinute,
                    state.HomePossession
                    ? "Home wins possession"
                    : "Away wins possession"
                });
            }

            float moveX =
                horizontalMovement(generator);

            float moveY =
                verticalMovement(generator);

            if (state.HomePossession)
            {
                moveX += 1.2f;
                homePossessionTicks++;
            }
            else
            {
                moveX -= 1.2f;
                awayPossessionTicks++;
            }

            state.BallPosition.X += moveX;
            state.BallPosition.Y += moveY;

            if (passChance(generator) < 18)
            {
                result.Events.push_back({
                    currentMinute,
                    state.HomePossession
                    ? "Home completes a pass"
                    : "Away completes a pass"
                });
            }

            if (shotChance(generator) < 4)
            {
                if (state.HomePossession)
                {
                    result.HomeShots++;

                    result.Events.push_back({
                        currentMinute,
                        "Home attempt on goal"
                    });

                    state.BallPosition.X += 10.0f;

                    if (state.BallPosition.X > 90.0f)
                    {
                        result.HomeShotsOnTarget++;

                        result.Events.push_back({
                            currentMinute,
                            "Home shot on target"
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

                    state.BallPosition.X -= 10.0f;

                    if (state.BallPosition.X < 15.0f)
                    {
                        result.AwayShotsOnTarget++;

                        result.Events.push_back({
                            currentMinute,
                            "Away shot on target"
                        });
                    }
                }
            }

            if (!goalScoredThisTick)
            {
                if (state.BallPosition.X >= 105.0f)
                {
                    result.HomeScore++;

                    result.Events.push_back({
                        currentMinute,
                        "Home scores"
                    });

                    state.BallPosition.X = 52.5f;
                    state.BallPosition.Y = 34.0f;

                    state.HomePossession = false;

                    goalScoredThisTick = true;
                }
                else if (state.BallPosition.X <= 0.0f)
                {
                    result.AwayScore++;

                    result.Events.push_back({
                        currentMinute,
                        "Away scores"
                    });

                    state.BallPosition.X = 52.5f;
                    state.BallPosition.Y = 34.0f;

                    state.HomePossession = true;

                    goalScoredThisTick = true;
                }
            }

            state.BallPosition.X = std::clamp(
                state.BallPosition.X,
                0.0f,
                105.0f
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