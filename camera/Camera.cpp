#include "raylib.h"
#include "Camera.h"

#include <algorithm>
#include <cmath>

void GameCamera::initialise(
    float worldPixelWidth,
    float worldPixelHeight,
    int screenWidth,
    int screenHeight
)
{
    raylib.target = {
        worldPixelWidth / 2.0f,
        worldPixelHeight / 2.0f
    };

    raylib.offset = {
        screenWidth / 2.0f,
        screenHeight / 2.0f
    };

    raylib.zoom = 1.0f;
}

void GameCamera::update(
    float dt,
    int,
    int
)
{
    float speed =
        900.0f *
        dt /
        raylib.zoom;

    if (
        IsKeyDown(KEY_A) ||
        IsKeyDown(KEY_LEFT)
    )
    {
        raylib.target.x -= speed;
    }

    if (
        IsKeyDown(KEY_D) ||
        IsKeyDown(KEY_RIGHT)
    )
    {
        raylib.target.x += speed;
    }

    if (
        IsKeyDown(KEY_W) ||
        IsKeyDown(KEY_UP)
    )
    {
        raylib.target.y -= speed;
    }

    if (
        IsKeyDown(KEY_S) ||
        IsKeyDown(KEY_DOWN)
    )
    {
        raylib.target.y += speed;
    }

    // Deliberately NO world-edge clamping.
    //
    // The camera is allowed to move completely outside
    // the generated world. Empty space is rendered black.

    float wheel =
        GetMouseWheelMove();

    if (wheel != 0.0f)
    {
        zoomAtMouse(
            GetMousePosition(),
            wheel
        );
    }
}

void GameCamera::zoomAtMouse(
    Vector2 mousePosition,
    float wheel
)
{
    Vector2 before =
        GetScreenToWorld2D(
            mousePosition,
            raylib
        );

    raylib.zoom *=
        wheel > 0.0f
        ? 1.12f
        : 0.89f;

    raylib.zoom =
        std::clamp(
            raylib.zoom,
            0.20f,
            5.0f
        );

    Vector2 after =
        GetScreenToWorld2D(
            mousePosition,
            raylib
        );

    raylib.target.x +=
        before.x -
        after.x;

    raylib.target.y +=
        before.y -
        after.y;
}

Vector2 GameCamera::screenToWorld(
    Vector2 screen
) const
{
    return GetScreenToWorld2D(
        screen,
        raylib
    );
}
