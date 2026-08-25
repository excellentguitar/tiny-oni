#include "raylib.h"

#include "core/World.h"
#include "character/Character.h"
#include "camera/Camera.h"
#include "rendering/WorldRenderer.h"
#include "ui/UI.h"
#include "persistence/Save.h"
#include "rendering/Sky.h"

#include <cstdlib>
#include <ctime>
#include <vector>

Sound jumpSound;


int main()
{
    srand(
        static_cast<unsigned>(
            time(nullptr)
        )
    );

    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;

    InitWindow(
        SCREEN_W,
        SCREEN_H,
        "Tiny ONI"
    );
InitAudioDevice();
jumpSound = LoadSound("sounds/boing.mp3");

if (!IsSoundValid(jumpSound))
{
    printf("ERROR: Could not load boing.mp3\n");
}
else
{
    printf("Loaded boing.mp3 OK\n");
}



    SetTargetFPS(60);

    World world;

    world.generate();
	Character player;

int spawnX =
    WORLD_W / 2;

float spawnY =
    100.0f;

player.initialise({
    spawnX * TILE_SIZE +
        TILE_SIZE / 2.0f,

    spawnY
});

	
	std::printf(
    "[WORLD] W=%d H=%d TILE=%d PIXELS=%dx%d\n",
    WORLD_W,
    WORLD_H,
    TILE_SIZE,
    WORLD_W * TILE_SIZE,
    WORLD_H * TILE_SIZE
);

std::printf(
    "[WORLD] centre tile=(%d,%d) type=%d\n",
    WORLD_W / 2,
    WORLD_H / 2,
    static_cast<int>(
        world.at(WORLD_W / 2, WORLD_H / 2).type
    )
);



    GameCamera camera;

    camera.initialise(
        WORLD_W * TILE_SIZE,
        WORLD_H * TILE_SIZE,
        SCREEN_W,
        SCREEN_H
    );
	std::printf(
    "[CAMERA] X=%.2f Y=%.2f Zoom=%.3f\n",
    camera.raylib.target.x,
    camera.raylib.target.y,
    camera.raylib.zoom
);

std::printf(
    "[WORLD] size=%zu tiles\n",
    world.cells.size()
);

    WorldRenderer renderer;
    GameUI ui;

    bool paused = false;
    bool timePaused = false;
    bool showUI = true;

    constexpr float DAY_LENGTH =
        30.0f;

    float timeOfDay =
        0.25f;

    float accumulator =
        0.0f;

    float simulationInterval =
        0.08f;

    std::vector<RainDrop> rain;

    rain.reserve(700);

    // --------------------------------------------------------
    // Rain spawning
    // --------------------------------------------------------

    auto spawnRain =
        [&]()
    {
        if (rain.size() >= 700)
            return;

        for (int i = 0; i < 4; ++i)
        {
            RainDrop drop;

            drop.x =
                static_cast<float>(
                    rand() % SCREEN_W
                );

            drop.y =
                -10.0f -
                static_cast<float>(
                    rand() % 100
                );

            drop.speed =
                260.0f +
                static_cast<float>(
                    rand() % 180
                );

            rain.push_back(drop);
        }
    };

    while (!WindowShouldClose())
    {
        float dt =
            GetFrameTime();
			
			
			
			// UPDATE SECTION
			
player.update(
    dt,
    world
);
			
			

        // ----------------------------------------------------
        // TIME
        // ----------------------------------------------------

        if (!timePaused)
        {
            timeOfDay +=
                dt / DAY_LENGTH;

            if (timeOfDay >= 1.0f)
                timeOfDay -= 1.0f;
        }

        if (IsKeyPressed(KEY_N))
        {
            timeOfDay += 0.05f;

            if (timeOfDay >= 1.0f)
                timeOfDay -= 1.0f;
        }

        if (IsKeyPressed(KEY_T))
            timePaused = !timePaused;

        if (IsKeyPressed(KEY_F1))
            showUI = !showUI;

        // ----------------------------------------------------
        // CAMERA
        // ----------------------------------------------------

        camera.update(
            dt,
            SCREEN_W,
            SCREEN_H,
            player.getPosition(),
            player.isMoving()
        );

        // ----------------------------------------------------
        // SIMULATION CONTROLS
        // ----------------------------------------------------

        if (IsKeyPressed(KEY_P))
            paused = !paused;

        if (IsKeyPressed(KEY_R))
        {
            world.generate();

            timeOfDay =
                0.25f;

            rain.clear();

            camera.raylib.target = {
                WORLD_W * TILE_SIZE / 2.0f,
                WORLD_H * TILE_SIZE / 2.0f
            };
        }

        if (IsKeyPressed(KEY_PERIOD))
            world.step();

        if (IsKeyPressed(KEY_ONE))
            simulationInterval = 0.16f;

        if (IsKeyPressed(KEY_TWO))
            simulationInterval = 0.08f;

        if (IsKeyPressed(KEY_THREE))
            simulationInterval = 0.04f;

        if (IsKeyPressed(KEY_FOUR))
            simulationInterval = 0.02f;

        // ----------------------------------------------------
        // SAVE
        //
        // F2 saves immediately to the next numbered slot.
        // ----------------------------------------------------

        if (IsKeyPressed(KEY_F2))
        {
            std::filesystem::create_directories(
                "saves"
            );

            int slot = 1;

            while (
                std::filesystem::exists(
                    "saves/world_" +
                    std::to_string(slot) +
                    ".oni"
                )
            )
            {
                ++slot;
            }

            std::string filename =
                "saves/world_" +
                std::to_string(slot) +
                ".oni";

            if (
                SaveSystem::saveWorld(
                    world,
                    filename
                )
            )
            {
                std::printf(
                    "Saved world: %s\n",
                    filename.c_str()
                );
            }
        }

        // ----------------------------------------------------
        // LOAD
        //
        // F3 opens the current simple console selector.
        // ----------------------------------------------------

        if (IsKeyPressed(KEY_F3))
        {
            std::vector<std::string> files =
                SaveSystem::findWorldFiles();

            if (!files.empty())
            {
                std::string filename =
                    SaveSystem::chooseWorldFile(
                        files
                    );

                if (!filename.empty())
                {
                    if (
                        SaveSystem::loadWorld(
                            world,
                            filename
                        )
                    )
                    {
                        rain.clear();

                        std::printf(
                            "Loaded world: %s\n",
                            filename.c_str()
                        );
                    }
                    else
                    {
                        std::printf(
                            "Failed to load world.\n"
                        );
                    }
                }
            }
            else
            {
                std::printf(
                    "No saved worlds found.\n"
                );
            }
        }

        // ----------------------------------------------------
        // MOUSE
        // ----------------------------------------------------

        Vector2 mouseWorld =
            camera.screenToWorld(
                GetMousePosition()
            );

        int tileX =
            static_cast<int>(
                std::floor(
                    mouseWorld.x /
                    TILE_SIZE
                )
            );

        int tileY =
            static_cast<int>(
                std::floor(
                    mouseWorld.y /
                    TILE_SIZE
                )
            );

        if (
            IsMouseButtonDown(
                MOUSE_BUTTON_LEFT
            )
        )
        {
            world.dig(
                tileX,
                tileY
            );
        }
		
		        if (IsKeyPressed(KEY_E))
        {
            world.chop(
                tileX,
                tileY
            );
        }

        if (
            IsMouseButtonDown(
                MOUSE_BUTTON_RIGHT
            )
        )
        {
            world.makeWater(
                tileX,
                tileY
            );
        }

        if (
            IsMouseButtonDown(
                MOUSE_BUTTON_MIDDLE
            )
        )
        {
            world.makeGas(
                tileX,
                tileY
            );
        }

        // ----------------------------------------------------
        // SIMULATION
        // ----------------------------------------------------

        if (!paused)
        {
            accumulator += dt;

            while (
                accumulator >=
                simulationInterval
            )
            {
                world.step();

                accumulator -=
                    simulationInterval;
            }
        }

        // ----------------------------------------------------
        // RAIN
        // ----------------------------------------------------

        bool daylight =
            timeOfDay > 0.20f &&
            timeOfDay < 0.80f;

        bool rainWeather =
            daylight &&
            std::sin(
                timeOfDay *
                PI *
                7.0f
            ) > 0.72f;

        if (rainWeather)
            spawnRain();

        // ----------------------------------------------------
        // RAIN MOVEMENT
        // ----------------------------------------------------

        for (
            size_t i = 0;
            i < rain.size();
        )
        {
            RainDrop& drop =
                rain[i];

            drop.y +=
                drop.speed *
                dt;

            bool remove =
                drop.y >
                SCREEN_H + 20;

            if (!remove)
            {
                Vector2 worldPosition =
                    camera.screenToWorld(
                        {
                            drop.x,
                            drop.y
                        }
                    );

                int gx =
                    static_cast<int>(
                        std::floor(
                            worldPosition.x /
                            TILE_SIZE
                        )
                    );

                int gy =
                    static_cast<int>(
                        std::floor(
                            worldPosition.y /
                            TILE_SIZE
                        )
                    );

                if (
                    world.inside(gx, gy) &&
                    world.solid(gx, gy)
                )
                {
                    int waterY =
                        gy - 1;

                    if (
                        world.inside(
                            gx,
                            waterY
                        )
                    )
                    {
                        world.makeWater(
                            gx,
                            waterY
                        );
                    }

                    remove = true;
                }
            }

            if (remove)
            {
                rain[i] =
                    rain.back();

                rain.pop_back();
            }
            else
            {
                ++i;
            }
        }

        // ----------------------------------------------------
        // DRAW
        // ----------------------------------------------------

        BeginDrawing();

        // Outside the world is deliberately BLACK.
        ClearBackground(
            {0, 0, 0, 255}
        );

        // Sky only exists where the camera is actually
        // above the generated world.
        //
        // The world itself is drawn over the sky.
        //
        // We calculate whether the camera view intersects
        // the atmosphere before drawing it.


Vector2 cameraTop =
    camera.screenToWorld(
        {0, 0}
    );

Vector2 cameraBottom =
    camera.screenToWorld(
        {
            static_cast<float>(SCREEN_W),
            static_cast<float>(SCREEN_H)
        }
    );

bool viewHasSky =
    cameraBottom.y > 0.0f &&
    cameraTop.y <
        WORLD_H * TILE_SIZE;

if (viewHasSky)
{
    drawSky(
        SCREEN_W,
        SCREEN_H,
        timeOfDay
    );
}


        BeginMode2D(
            camera.raylib
        );

        renderer.draw(
            world,
            camera,
            SCREEN_W,
            SCREEN_H,
            tileX,
            tileY
        );
		player.draw();


        EndMode2D();

        // ----------------------------------------------------
        // RAIN
        // ----------------------------------------------------

        if (daylight)
        {
            for (
                const RainDrop& drop :
                rain
            )
            {
                DrawLine(
                    static_cast<int>(drop.x),
                    static_cast<int>(drop.y),
                    static_cast<int>(drop.x),
                    static_cast<int>(drop.y + 5),
                    {
                        175,
                        215,
                        245,
                        100
                    }
                );
            }
        }

        // ----------------------------------------------------
        // UI
        // ----------------------------------------------------

/*
std::printf(
    "[CAMERA] X=%.2f Y=%.2f Zoom=%.3f\n",
    camera.raylib.target.x,
    camera.raylib.target.y,
    camera.raylib.zoom
);*/
ui.draw(
    world,
    camera,
    SCREEN_W,
    SCREEN_H,
    tileX,
    tileY,
    paused,
    timePaused,
    showUI,
    timeOfDay,
    simulationInterval
);

        EndDrawing();
    }

UnloadSound(jumpSound);
CloseAudioDevice();
CloseWindow();

return 0;
}


// ============================================================
// END FILE: main.cpp
