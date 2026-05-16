#include <iostream>
#include <thread>
#include <chrono>

#include "Match/Match.h"

using namespace OpenPitch;

int main()
{
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
        7, 6, 5
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
        6, 7, 6
    };

    Match match(
        homeTeam,
        awayTeam
    );

    auto result = match.Play();

    constexpr int pitchWidth = 60;
    constexpr int totalRealMilliseconds = 12000;

    const int tickCount =
        static_cast<int>(
            result.BallHistory.size()
        );

    const int tickDelay =
        totalRealMilliseconds / tickCount;

    for (size_t i = 0;
         i < result.BallHistory.size();
         ++i)
    {
        const float currentMinute =
            (
                static_cast<float>(i)
                / static_cast<float>(tickCount)
            ) * 90.0f;

        std::cout << "\x1B[2J\x1B[H";

        std::cout
            << result.HomeTeam.Name
            << " "
            << result.HomeScoreHistory[i]
            << " - "
            << result.AwayScoreHistory[i]
            << " "
            << result.AwayTeam.Name
            << std::endl;

        std::cout
            << "Time: "
            << static_cast<int>(
                currentMinute
            )
            << "'"
            << std::endl;

        std::cout
            << "Possession: "
            << result.HomePossessionHistory[i]
            << "% - "
            << result.AwayPossessionHistory[i]
            << "%"
            << std::endl;

        std::cout
            << "Shots: "
            << result.HomeShotsHistory[i]
            << " - "
            << result.AwayShotsHistory[i]
            << std::endl;

        std::cout
            << "Shots On Target: "
            << result.HomeShotsOnTargetHistory[i]
            << " - "
            << result.AwayShotsOnTargetHistory[i]
            << std::endl;

        std::cout << std::endl;

        const auto& ballPosition =
            result.BallHistory[i];

        const int ballX =
            static_cast<int>(
                (
                    ballPosition.X
                    / 105.0f
                ) * static_cast<float>(
                    pitchWidth - 1
                )
            );

        std::string pitch(
            pitchWidth,
            '-'
        );

        pitch.front() = 'H';
        pitch.back() = 'A';

        if (
            ballX > 0
            && ballX < pitchWidth - 1
        )
        {
            pitch[ballX] = 'O';
        }

        std::cout
            << "["
            << pitch
            << "]"
            << std::endl;

        std::cout
            << "H = Home Goal | A = Away Goal | O = Ball"
            << std::endl;

        std::this_thread::sleep_for(
            std::chrono::milliseconds(
                tickDelay
            )
        );
    }

    return 0;
}