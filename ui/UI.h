#pragma once

#include "../camera/Camera.h"

class World;

class GameUI
{
public:

    void draw(
        const World& world,
        const GameCamera& camera,
        int screenWidth,
        int screenHeight,
        int cursorX,
        int cursorY,
        bool paused,
        bool timePaused,
        bool showUI,
        float timeOfDay,
        float simulationInterval
    );
};