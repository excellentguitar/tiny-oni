#include "Character.h"
#include "raylib.h"
#include "core/World.h"

#include <cmath>

extern Sound jumpSound;

void Character::initialise(
    Vector2 startPosition
)
{
    position = startPosition;

    velocity = {
        0.0f,
        0.0f
    };

    grounded = false;
}

void Character::update(
    float dt,
    const World& world
)
{
    if (dt > 0.05f)
        dt = 0.05f;

    float inputX = 0.0f;

    if (IsKeyDown(KEY_A))
        inputX -= 1.0f;

    if (IsKeyDown(KEY_D))
        inputX += 1.0f;

    velocity.x =
        inputX *
        moveSpeed;

    if (
        IsKeyPressed(KEY_SPACE) &&
        grounded
    )
    {
        velocity.y =
            -jumpSpeed;

        grounded = false;
		  PlaySound(jumpSound);
    }
	
	
	

    velocity.y +=
        gravity *
        dt;

    // --------------------------------------------------------
    // HORIZONTAL MOVEMENT
    // --------------------------------------------------------

    float dx =
        velocity.x *
        dt;

    if (dx != 0.0f)
    {
        float newX =
            position.x +
            dx;

        float checkX =
            dx > 0.0f
                ? newX + halfWidth
                : newX - halfWidth;

        int tileX =
            static_cast<int>(
                std::floor(
                    checkX /
                    TILE_SIZE
                )
            );

        int topTile =
            static_cast<int>(
                std::floor(
                    (
                        position.y -
                        halfHeight +
                        0.1f
                    ) /
                    TILE_SIZE
                )
            );

        int bottomTile =
            static_cast<int>(
                std::floor(
                    (
                        position.y +
                        halfHeight -
                        0.1f
                    ) /
                    TILE_SIZE
                )
            );

        bool blocked = false;

        for (
            int y = topTile;
            y <= bottomTile;
            ++y
        )
        {
            if (
                world.characterSolid(
                    tileX,
                    y
                )
            )
            {
                blocked = true;
                break;
            }
        }

        if (!blocked)
        {
            position.x =
                newX;
        }
        else
        {
            // ------------------------------------------------
            // STEP UP 1 OR 2 BLOCKS
            // ------------------------------------------------

            bool steppedUp = false;

            if (
                grounded &&
                velocity.y >= 0.0f
            )
            {
                constexpr int MAX_STEP_HEIGHT = 2;

                for (
                    int step = 1;
                    step <= MAX_STEP_HEIGHT;
                    ++step
                )
                {
                    float candidateY =
                        position.y -
                        step * TILE_SIZE;

                    int candidateTop =
                        static_cast<int>(
                            std::floor(
                                (
                                    candidateY -
                                    halfHeight +
                                    0.1f
                                ) /
                                TILE_SIZE
                            )
                        );

                    int candidateBottom =
                        static_cast<int>(
                            std::floor(
                                (
                                    candidateY +
                                    halfHeight -
                                    0.1f
                                ) /
                                TILE_SIZE
                            )
                        );

                    bool bodyClear = true;

                    for (
                        int y = candidateTop;
                        y <= candidateBottom;
                        ++y
                    )
                    {
                        if (
                            world.characterSolid(
                                tileX,
                                y
                            )
                        )
                        {
                            bodyClear = false;
                            break;
                        }
                    }

                    if (!bodyClear)
                        continue;

                    int groundTile =
                        static_cast<int>(
                            std::floor(
                                (
                                    candidateY +
                                    halfHeight +
                                    0.1f
                                ) /
                                TILE_SIZE
                            )
                        );

                    if (
                        !world.characterSolid(
                            tileX,
                            groundTile
                        )
                    )
                    {
                        continue;
                    }

                    position.x =
                        newX;

                    position.y =
                        groundTile *
                        TILE_SIZE -
                        halfHeight -
                        0.01f;

                    velocity.y =
                        0.0f;

                    grounded =
                        true;

                    steppedUp =
                        true;

                    break;
                }
            }

            if (!steppedUp)
            {
                velocity.x =
                    0.0f;
            }
        }
    }

    // --------------------------------------------------------
    // VERTICAL MOVEMENT
    // --------------------------------------------------------

    float dy =
        velocity.y *
        dt;

    grounded = false;

    if (dy != 0.0f)
    {
        float newY =
            position.y +
            dy;

        float checkY =
            dy > 0.0f
                ? newY + halfHeight
                : newY - halfHeight;

        int tileY =
            static_cast<int>(
                std::floor(
                    checkY /
                    TILE_SIZE
                )
            );

        int leftTile =
            static_cast<int>(
                std::floor(
                    (
                        position.x -
                        halfWidth +
                        0.1f
                    ) /
                    TILE_SIZE
                )
            );

        int rightTile =
            static_cast<int>(
                std::floor(
                    (
                        position.x +
                        halfWidth -
                        0.1f
                    ) /
                    TILE_SIZE
                )
            );

        bool blocked = false;

        for (
            int x = leftTile;
            x <= rightTile;
            ++x
        )
        {
            if (
                world.characterSolid(
                    x,
                    tileY
                )
            )
            {
                blocked = true;
                break;
            }
        }

        if (!blocked)
        {
            position.y =
                newY;
        }
        else
        {
            if (dy > 0.0f)
            {
                position.y =
                    tileY *
                    TILE_SIZE -
                    halfHeight -
                    0.01f;

                grounded =
                    true;
            }
            else
            {
                position.y =
                    (tileY + 1) *
                    TILE_SIZE +
                    halfHeight +
                    0.01f;
            }

            velocity.y =
                0.0f;
        }
    }

    // --------------------------------------------------------
    // WORLD X LIMITS
    // --------------------------------------------------------

    if (
        position.x <
        halfWidth
    )
    {
        position.x =
            halfWidth;

        velocity.x =
            0.0f;
    }

    float worldWidth =
        WORLD_W *
        TILE_SIZE;

    if (
        position.x >
        worldWidth -
        halfWidth
    )
    {
        position.x =
            worldWidth -
            halfWidth;

        velocity.x =
            0.0f;
    }
}

void Character::draw() const
{
    // Head
    DrawCircle(
        static_cast<int>(
            position.x
        ),
        static_cast<int>(
            position.y - 8.0f
        ),
        5.0f,
        GREEN
    );

    // Nose
    DrawCircle(
        static_cast<int>(
            position.x + 5.0f
        ),
        static_cast<int>(
            position.y - 8.0f
        ),
        1.5f,
        GREEN
    );

    // Body
    DrawRectangle(
        static_cast<int>(
            position.x - 3.5f
        ),
        static_cast<int>(
            position.y - 3.0f
        ),
        7,
        10,
        BLUE
    );

    // Left foot
    DrawRectangle(
        static_cast<int>(
            position.x - 4.0f
        ),
        static_cast<int>(
            position.y + 7.0f
        ),
        3,
        2,
        BLUE
    );

    // Right foot
    DrawRectangle(
        static_cast<int>(
            position.x + 1.0f
        ),
        static_cast<int>(
            position.y + 7.0f
        ),
        3,
        2,
        BLUE
    );
}

Vector2 Character::getPosition() const
{
    return position;
}

bool Character::isMoving() const
{
    return velocity.x != 0.0f;
}