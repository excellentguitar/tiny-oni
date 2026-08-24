// ============================================================

#pragma once

#include "raylib.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static constexpr int WORLD_W = 960;
static constexpr int WORLD_H = 140;
static constexpr int TILE_SIZE = 10;

enum class Type : uint8_t
{
    Empty,
    Oxygen,

    Grass,
    Dirt,
    Clay,
    Claystone,
    Sand,
    Sandstone,
    Chalk,
    Limestone,

    Granite,
    Basalt,
    Gravel,

    Copper,
    Iron,
    Coal,

    Water,
    Gas,
    Magma,

    Wood,
    Leaves
};

struct Tile
{
    Type type = Type::Empty;

    float amount = 0.0f;
    float pressure = 0.0f;
    float temperature = 22.0f;
};

struct RainDrop
{
    float x = 0.0f;
    float y = 0.0f;
    float speed = 300.0f;
};


// ============================================================
// END FILE: core/Types.h
// ============================================================
