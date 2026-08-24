#pragma once

#include "raylib.h"

class World;
class GameCamera;

class WorldRenderer
{
public:

    WorldRenderer();

    void draw(
        const World& world,
        const GameCamera& camera,
        int screenWidth,
        int screenHeight,
        int cursorX,
        int cursorY
    );

private:

    Color leafColor(
        int kind
    ) const;

    void drawTile(
        const World& world,
        int x,
        int y
    ) const;
};