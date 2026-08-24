#include "raylib.h"

#include "Sky.h"

#include <algorithm>
#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846
#endif
RGB blend(
    RGB a,
    RGB b,
    float t
)
{
    t =
        std::clamp(
            t,
            0.0f,
            1.0f
        );

    return {
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t
    };
}

Color rgbColor(RGB c)
{
    return {
        static_cast<unsigned char>(
            std::clamp(
                static_cast<int>(c.r),
                0,
                255
            )
        ),

        static_cast<unsigned char>(
            std::clamp(
                static_cast<int>(c.g),
                0,
                255
            )
        ),

        static_cast<unsigned char>(
            std::clamp(
                static_cast<int>(c.b),
                0,
                255
            )
        ),

        255
    };
}

// Mostly blue / dark blue.
// Sunrise and sunset are deliberately short.

RGB skyAtTime(float t)
{
    t -= std::floor(t);

    const RGB nightBlack =
        {3, 5, 14};

    const RGB deepBlue =
        {12, 32, 75};

    const RGB blue =
        {48, 94, 150};

    const RGB daylight =
        {105, 165, 205};

    const RGB sunsetBlue =
        {70, 105, 160};

    const RGB subtlePink =
        {150, 105, 130};

    const RGB subtleOrange =
        {185, 115, 75};

    // Night.
    if (t < 0.16f)
    {
        float p =
            t / 0.16f;

        return blend(
            nightBlack,
            deepBlue,
            p
        );
    }

    // Short dawn.
    if (t < 0.20f)
    {
        float p =
            (t - 0.16f) / 0.04f;

        return blend(
            deepBlue,
            subtlePink,
            p
        );
    }

    if (t < 0.23f)
    {
        float p =
            (t - 0.20f) / 0.03f;

        return blend(
            subtlePink,
            daylight,
            p
        );
    }

    // Long blue daylight.
    if (t < 0.57f)
    {
        return daylight;
    }

    // Short sunset.
    if (t < 0.61f)
    {
        float p =
            (t - 0.57f) / 0.04f;

        return blend(
            daylight,
            sunsetBlue,
            p
        );
    }

    if (t < 0.64f)
    {
        float p =
            (t - 0.61f) / 0.03f;

        return blend(
            sunsetBlue,
            subtleOrange,
            p
        );
    }

    if (t < 0.68f)
    {
        float p =
            (t - 0.64f) / 0.04f;

        return blend(
            subtleOrange,
            deepBlue,
            p
        );
    }

    return blend(
        deepBlue,
        nightBlack,
        (t - 0.68f) / 0.32f
    );
}

void drawSky(
    int screenWidth,
    int screenHeight,
    float timeOfDay
)
{
    RGB base =
        skyAtTime(timeOfDay);

    for (
        int y = 0;
        y < screenHeight;
        ++y
    )
    {
        float vertical =
            y /
            static_cast<float>(screenHeight);

        RGB top =
            blend(
                base,
                {2, 3, 10},
                0.28f *
                (1.0f - vertical)
            );

        RGB horizon =
            blend(
                top,
                base,
                vertical * 0.55f
            );

        DrawRectangle(
            0,
            y,
            screenWidth,
            1,
            rgbColor(horizon)
        );
    }

    float night = 0.0f;

    if (timeOfDay < 0.18f)
    {
        night =
            1.0f -
            timeOfDay / 0.18f;
    }
    else if (timeOfDay > 0.66f)
    {
        night =
            (timeOfDay - 0.66f) /
            0.34f;
    }

    night =
        std::clamp(
            night,
            0.0f,
            1.0f
        );

    if (night > 0.01f)
    {
        for (int i = 0; i < 110; ++i)
        {
            int x =
                (i * 7919) %
                screenWidth;

            int y =
                15 +
                (
                    i * 3571
                ) %
                std::max(
                    1,
                    screenHeight / 2
                );

            int size =
                i % 11 == 0
                ? 2
                : 1;

            unsigned char alpha =
                static_cast<unsigned char>(
                    night * 230.0f
                );

            DrawRectangle(
                x,
                y,
                size,
                size,
                {
                    255,
                    245,
                    220,
                    alpha
                }
            );
        }
    }

    // Sun.

    if (
        timeOfDay >= 0.20f &&
        timeOfDay <= 0.80f
    )
    {
        float p =
            (timeOfDay - 0.20f) /
            0.60f;

        float angle =
            p * PI;

        float x =
            p *
            (screenWidth + 180.0f) -
            90.0f;

        float horizon =
            screenHeight * 0.68f;

        float y =
            horizon -
            std::sin(angle) *
            screenHeight *
            0.48f;

        DrawCircle(
            static_cast<int>(x),
            static_cast<int>(y),
            35,
            {
                255,
                180,
                70,
                28
            }
        );

        DrawCircle(
            static_cast<int>(x),
            static_cast<int>(y),
            23,
            {
                255,
                210,
                75,
                245
            }
        );
    }

    // Moon.

    float moonP =
        timeOfDay + 0.5f;

    moonP -=
        std::floor(moonP);

    if (
        moonP >= 0.20f &&
        moonP <= 0.80f
    )
    {
        float p =
            (moonP - 0.20f) /
            0.60f;

        float angle =
            p * PI;

        float x =
            p *
            (screenWidth + 180.0f) -
            90.0f;

        float horizon =
            screenHeight * 0.68f;

        float y =
            horizon -
            std::sin(angle) *
            screenHeight *
            0.48f;

        DrawCircle(
            static_cast<int>(x),
            static_cast<int>(y),
            18,
            {
                225,
                230,
                210,
                255
            }
        );

        DrawCircle(
            static_cast<int>(x + 7),
            static_cast<int>(y - 4),
            15,
            rgbColor(base)
        );
    }
}


// ============================================================
// END FILE: rendering/Sky.cpp
// ============================================================
