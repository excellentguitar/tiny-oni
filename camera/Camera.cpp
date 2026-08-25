#include "raylib.h"
#include "Camera.h"
#include <algorithm>

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

    raylib.rotation = 0.0f;
    raylib.zoom = 1.0f;
}

void GameCamera::update(
    float dt,
    int,
    int,
    Vector2 playerPosition,
    bool playerMoving
)
{
    float speed = 900.0f * dt / raylib.zoom;

    if (playerMoving)
    {
        raylib.target.x = playerPosition.x;
        raylib.target.y = playerPosition.y;
    }
    else
    {
        if (IsKeyDown(KEY_LEFT))
            raylib.target.x -= speed;

        if (IsKeyDown(KEY_RIGHT))
            raylib.target.x += speed;

        if (IsKeyDown(KEY_UP))
            raylib.target.y -= speed;

        if (IsKeyDown(KEY_DOWN))
            raylib.target.y += speed;
    }

    float wheel = GetMouseWheelMove();

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

    raylib.target.x += before.x - after.x;
    raylib.target.y += before.y - after.y;
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