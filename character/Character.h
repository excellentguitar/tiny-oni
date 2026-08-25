#pragma once

#include "raylib.h"

class World;

class Character
{
public:

    void initialise(
        Vector2 startPosition
    );

    void update(
        float dt,
        const World& world
    );

    void draw() const;

    Vector2 getPosition() const;

private:

    Vector2 position{};
    Vector2 velocity{};

    float moveSpeed = 220.0f;

    float gravity = 900.0f;

    float jumpSpeed = 380.0f;

    bool grounded = false;

    // Collision body.
    float halfWidth = 3.0f;
    float halfHeight = 7.0f;
};