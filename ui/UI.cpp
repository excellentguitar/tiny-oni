// ============================================================
#include "UI.h"

#include <cstdio>

#include "raylib.h"

#include "../core/Types.h"
#include "../core/Materials.h"
#include "../core/World.h"
void GameUI::draw(
    const World& world,
    int screenWidth,
    int screenHeight,
    int cursorX,
    int cursorY,
    bool paused,
    bool timePaused,
    bool showUI,
    float timeOfDay,
    float interval
)
{
    if (!showUI)
        return;

    DrawRectangle(
        8,
        8,
        650,
        155,
        {
            0,
            0,
            0,
            185
        }
    );

    DrawText(
        "LMB dig   RMB water   MMB gas",
        16,
        16,
        16,
        RAYWHITE
    );

    DrawText(
        "WASD/arrows pan   wheel zoom   SPACE pause",
        16,
        37,
        14,
        RAYWHITE
    );

    DrawText(
        ". step   R regenerate   1/2/3/4 speed",
        16,
        57,
        14,
        RAYWHITE
    );

    DrawText(
        "T pause time   N advance time   F1 hide UI",
        16,
        77,
        14,
        RAYWHITE
    );

    DrawText(
        paused
            ? "SIMULATION PAUSED"
            : "SIMULATION RUNNING",
        16,
        101,
        14,
        paused
            ? YELLOW
            : GREEN
    );

    DrawText(
        timePaused
            ? "TIME PAUSED"
            : "TIME RUNNING",
        220,
        101,
        14,
        timePaused
            ? ORANGE
            : SKYBLUE
    );

    int totalMinutes =
        static_cast<int>(
            timeOfDay *
            24.0f *
            60.0f
        );

    int hours =
        totalMinutes / 60;

    int minutes =
        totalMinutes % 60;

    char clockText[32];

    std::snprintf(
        clockText,
        sizeof(clockText),
        "%02d:%02d",
        hours,
        minutes
    );

    DrawText(
        clockText,
        16,
        124,
        22,
        RAYWHITE
    );

    DrawText(
        "30 second day",
        90,
        128,
        13,
        LIGHTGRAY
    );

    DrawText(
        "Atmosphere: OXYGEN",
        210,
        128,
        13,
        {
            120,
            210,
            240,
            255
        }
    );

    char waterText[64];

    std::snprintf(
        waterText,
        sizeof(waterText),
        "Water: %.2f",
        world.totalWater()
    );

    DrawText(
        waterText,
        405,
        128,
        13,
        SKYBLUE
    );

    if (
        world.inside(
            cursorX,
            cursorY
        )
    )
    {
        const Tile& tile =
            world.at(
                cursorX,
                cursorY
            );

        DrawRectangle(
            screenWidth - 230,
            8,
            222,
            105,
            {
                0,
                0,
                0,
                190
            }
        );

        DrawText(
            materialName(tile.type),
            screenWidth - 215,
            16,
            18,
            RAYWHITE
        );

        char buffer[128];

        std::snprintf(
            buffer,
            sizeof(buffer),
            "x %d  y %d",
            cursorX,
            cursorY
        );

        DrawText(
            buffer,
            screenWidth - 215,
            39,
            14,
            RAYWHITE
        );

        std::snprintf(
            buffer,
            sizeof(buffer),
            "amount %.3f",
            tile.amount
        );

        DrawText(
            buffer,
            screenWidth - 215,
            59,
            14,
            RAYWHITE
        );

        std::snprintf(
            buffer,
            sizeof(buffer),
            "temp %.1f",
            tile.temperature
        );

        DrawText(
            buffer,
            screenWidth - 215,
            79,
            14,
            RAYWHITE
        );
    }
}


// ============================================================
// END FILE: ui/UI.cpp
// ============================================================
