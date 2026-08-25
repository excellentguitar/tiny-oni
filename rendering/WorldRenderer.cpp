// ============================================================
#include "WorldRenderer.h"

#include <algorithm>
#include <cmath>
#include <cstdio>

#include "raylib.h"

#include "../core/Types.h"
#include "../core/Materials.h"
#include "../core/World.h"
#include "../camera/Camera.h"

WorldRenderer::WorldRenderer()
{
}

Color WorldRenderer::leafColor(
    int kind
) const
{
    switch (kind)
    {
        case 0:
            // Oak
            return {65, 125, 48, 255};

        case 1:
            // Pine
            return {28, 91, 58, 255};

        case 2:
            // Birch - deliberately pale.
            return {175, 190, 145, 255};

        case 3:
            // Redwood
            return {48, 118, 66, 255};

        case 4:
            // Acacia
            return {105, 155, 55, 255};

        case 5:
            // Purpleheart
            return {63, 116, 82, 255};

        case 6:
            // Ebony - very dark.
            return {25, 55, 30, 255};

        default:
            return {55, 125, 55, 255};
    }
}

void WorldRenderer::drawTile(
    const World& world,
    int x,
    int y
) const
{
    const Tile& tile =
        world.at(x, y);

if (
    tile.type == Type::Empty ||
    tile.type == Type::Oxygen
)
{
    if (y > world.surface[x])
    {
        DrawRectangle(
            x * TILE_SIZE,
            y * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE,
            BLACK
        );
    }

    return;
}

    int px =
        x * TILE_SIZE;

    int py =
        y * TILE_SIZE;

    if (tile.type == Type::Water)
    {
        Color water =
            materialColor(
                Type::Water
            );

        int height =
            std::max(
                1,
                static_cast<int>(
                    TILE_SIZE *
                    tile.amount
                )
            );

        DrawRectangle(
            px,
            py + TILE_SIZE - height,
            TILE_SIZE,
            height,
            water
        );

        if (tile.amount > 0.15f)
        {
            DrawRectangle(
                px + 1,
                py + TILE_SIZE - height,
                TILE_SIZE - 2,
                1,
                {
                    150,
                    215,
                    255,
                    130
                }
            );
        }

        return;
    }

    if (tile.type == Type::Gas)
    {
        unsigned char alpha =
            static_cast<unsigned char>(
                std::clamp(
                    static_cast<int>(
                        35.0f +
                        tile.amount * 35.0f
                    ),
                    15,
                    70
                )
            );

        DrawCircle(
            px + TILE_SIZE / 2,
            py + TILE_SIZE / 2,
            1.5f,
            {
                135,
                205,
                235,
                alpha
            }
        );

        return;
    }

    if (tile.type == Type::Magma)
    {
        DrawRectangle(
            px,
            py,
            TILE_SIZE,
            TILE_SIZE,
            materialColor(
                Type::Magma
            )
        );

        if ((x + y) % 5 == 0)
        {
            DrawRectangle(
                px + 2,
                py + 2,
                3,
                3,
                {
                    255,
                    180,
                    45,
                    255
                }
            );
        }

        return;
    }

    if (tile.type == Type::Leaves)
    {
        int kind =
            std::clamp(
                static_cast<int>(
                    std::round(
                        tile.temperature -
                        100.0f
                    )
                ),
                0,
                6
            );

        Color leaf =
            leafColor(kind);

        DrawRectangle(
            px,
            py,
            TILE_SIZE,
            TILE_SIZE,
            leaf
        );

        if ((x + y) % 4 == 0)
        {
            DrawRectangle(
                px + 2,
                py + 2,
                3,
                3,
                {
                    static_cast<unsigned char>(
                        std::min(
                            255,
                            leaf.r + 18
                        )
                    ),

                    static_cast<unsigned char>(
                        std::min(
                            255,
                            leaf.g + 18
                        )
                    ),

                    static_cast<unsigned char>(
                        std::min(
                            255,
                            leaf.b + 12
                        )
                    ),

                    255
                }
            );
        }

        return;
    }

    Color base =
        materialColor(
            tile.type
        );

    DrawRectangle(
        px,
        py,
        TILE_SIZE,
        TILE_SIZE,
        base
    );

    int n =
        (
            x * 17 +
            y * 31
        ) % 9;

    if (n == 0 || n == 1)
    {
        Color variation =
            base;

        variation.r =
            static_cast<unsigned char>(
                std::min(
                    255,
                    variation.r + 10
                )
            );

        variation.g =
            static_cast<unsigned char>(
                std::min(
                    255,
                    variation.g + 8
                )
            );

        variation.b =
            static_cast<unsigned char>(
                std::min(
                    255,
                    variation.b + 8
                )
            );

        DrawRectangle(
            px + 2,
            py + 2,
            3,
            3,
            variation
        );
    }

    if (tile.type == Type::Copper)
    {
        DrawRectangle(
            px + 2,
            py + 2,
            3,
            3,
            {
                55,
                180,
                145,
                255
            }
        );
    }

    if (
        tile.type == Type::Iron &&
        (x + y) % 3 == 0
    )
    {
        DrawRectangle(
            px + 3,
            py + 4,
            3,
            2,
            {
                185,
                100,
                75,
                255
            }
        );
    }

    if (tile.type == Type::Coal)
    {
        DrawRectangle(
            px + 2,
            py + 2,
            4,
            4,
            {
                20,
                20,
                22,
                255
            }
        );
    }

    if (tile.type == Type::Wood)
    {
        DrawRectangle(
            px + 3,
            py,
            2,
            TILE_SIZE,
            {
                70,
                42,
                27,
                180
            }
        );
    }

    DrawRectangleLines(
        px,
        py,
        TILE_SIZE,
        TILE_SIZE,
        {
            35,
            35,
            40,
            255
        }
    );
}

void WorldRenderer::draw(
    const World& world,
    const GameCamera& camera,
    int screenWidth,
    int screenHeight,
    int cursorX,
    int cursorY
)
{
    Vector2 topLeft =
        GetScreenToWorld2D(
            {0, 0},
            camera.raylib
        );

    Vector2 bottomRight =
        GetScreenToWorld2D(
            {
                static_cast<float>(screenWidth),
                static_cast<float>(screenHeight)
            },
            camera.raylib
        );

 int minX = static_cast<int>(
    std::floor(
        std::min(topLeft.x, bottomRight.x) / TILE_SIZE
    )
) - 2;

int maxX = static_cast<int>(
    std::floor(
        std::max(topLeft.x, bottomRight.x) / TILE_SIZE
    )
) + 2;

int minY = static_cast<int>(
    std::floor(
        std::min(topLeft.y, bottomRight.y) / TILE_SIZE
    )
) - 2;

int maxY = static_cast<int>(
    std::floor(
        std::max(topLeft.y, bottomRight.y) / TILE_SIZE
    )
) + 2;

std::printf(
    "[RENDER] tiles X=%d..%d Y=%d..%d\n",
    minX,
    maxX,
    minY,
    maxY
);

    // Large buffer around the world.
    //
    // Anything outside the generated world stays black.
    // The camera is therefore free to travel everywhere.

std::printf(
    "[RENDER] camera world TL=(%.1f, %.1f) BR=(%.1f, %.1f)\n",
    topLeft.x,
    topLeft.y,
    bottomRight.x,
    bottomRight.y
);

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            if (!world.inside(x, y))
                continue;

            drawTile(
                world,
                x,
                y
            );
        }
    }

    if (world.inside(cursorX, cursorY))
    {
        DrawRectangleLines(
            cursorX * TILE_SIZE,
            cursorY * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE,
            YELLOW
        );
    }
}

// ============================================================
// END FILE: rendering/WorldRenderer.cpp
// ============================================================