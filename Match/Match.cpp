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

        Vector2 ballPosition{
            52.5f,
            34.0f
        };

        bool homePossession = true;
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
                homePossession = !homePossession;

                result.Events.push_back({
                    currentMinute,
                    homePossession
                    ? "Home wins possession"
                    : "Away wins possession"
                });
            }

            float moveX =
                horizontalMovement(generator);

            float moveY =
                verticalMovement(generator);

            if (homePossession)
            {
                moveX += 1.2f;
                homePossessionTicks++;
            }
            else
            {
                moveX -= 1.2f;
                awayPossessionTicks++;
            }

            ballPosition.X += moveX;
            ballPosition.Y += moveY;

            if (passChance(generator) < 18)
            {
                result.Events.push_back({
                    currentMinute,
                    homePossession
                    ? "Home completes a pass"
                    : "Away completes a pass"
                });
            }

            if (shotChance(generator) < 4)
            {
                if (homePossession)
                {
                    result.HomeShots++;

                    result.Events.push_back({
                        currentMinute,
                        "Home attempt on goal"
                    });

                    ballPosition.X += 10.0f;

                    if (ballPosition.X > 90.0f)
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

                    ballPosition.X -= 10.0f;

                    if (ballPosition.X < 15.0f)
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
                if (ballPosition.X >= 105.0f)
                {
                    result.HomeScore++;

                    result.Events.push_back({
                        currentMinute,
                        "Home scores"
                    });

                    ballPosition.X = 52.5f;
                    ballPosition.Y = 34.0f;

                    homePossession = false;

                    goalScoredThisTick = true;
                }
                else if (ballPosition.X <= 0.0f)
                {
                    result.AwayScore++;

                    result.Events.push_back({
                        currentMinute,
                        "Away scores"
                    });

                    ballPosition.X = 52.5f;
                    ballPosition.Y = 34.0f;

                    homePossession = true;

                    goalScoredThisTick = true;
                }
            }

            ballPosition.X = std::clamp(
                ballPosition.X,
                0.0f,
                105.0f
            );

            ballPosition.Y = std::clamp(
                ballPosition.Y,
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

            result.HomePossessionHistory.push_back(
                result.HomePossession
            );

            result.AwayPossessionHistory.push_back(
                result.AwayPossession
            );

            result.HomeScoreHistory.push_back(
                result.HomeScore
            );

            result.AwayScoreHistory.push_back(
                result.AwayScore
            );

            result.HomeShotsHistory.push_back(
                result.HomeShots
            );

            result.AwayShotsHistory.push_back(
                result.AwayShots
            );

            result.HomeShotsOnTargetHistory.push_back(
                result.HomeShotsOnTarget
            );

            result.AwayShotsOnTargetHistory.push_back(
                result.AwayShotsOnTarget
            );

            result.Events.push_back({
                currentMinute,
                homePossession
                ? "Home in possession"
                : "Away in possession"
            });

            result.BallHistory.push_back(
                ballPosition
            );
        }

        return result;
    }
}