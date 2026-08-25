#pragma once

#include "raylib.h"

class GameCamera
{
public:

    Camera2D raylib;

    void initialise(
        float worldWidth,
        float worldHeight,
        int screenWidth,
        int screenHeight
    );

    void update(
        float dt,
        int screenWidth,
        int screenHeight
    );

    Vector2 screenToWorld(
        Vector2 screenPosition
    ) const;

private:

    void zoomAtMouse(
        Vector2 mousePosition,
        float wheel
    );
};