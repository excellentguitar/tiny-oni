// ============================================================
#include "Save.h"

#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "../core/Types.h"
#include "../core/World.h"
namespace SaveSystem
{

static constexpr uint32_t SAVE_MAGIC =
    0x544F4E49; // "TONI"

static constexpr uint32_t SAVE_VERSION = 1;

struct SaveHeader
{
    uint32_t magic;
    uint32_t version;

    uint32_t width;
    uint32_t height;
};

bool saveWorld(
    const World& world,
    const std::string& filename
)
{
    std::filesystem::create_directories(
        "saves"
    );

    std::ofstream file(
        filename,
        std::ios::binary
    );

    if (!file)
        return false;

    SaveHeader header{
        SAVE_MAGIC,
        SAVE_VERSION,
        WORLD_W,
        WORLD_H
    };

    file.write(
        reinterpret_cast<const char*>(&header),
        sizeof(header)
    );

    file.write(
        reinterpret_cast<const char*>(
            world.surface.data()
        ),
        sizeof(int) *
        world.surface.size()
    );

    file.write(
        reinterpret_cast<const char*>(
            world.cells.data()
        ),
        sizeof(Tile) *
        world.cells.size()
    );

    return file.good();
}

bool loadWorld(
    World& world,
    const std::string& filename
)
{
    std::ifstream file(
        filename,
        std::ios::binary
    );

    if (!file)
        return false;

    SaveHeader header{};

    file.read(
        reinterpret_cast<char*>(&header),
        sizeof(header)
    );

    if (
        header.magic != SAVE_MAGIC ||
        header.version != SAVE_VERSION ||
        header.width != WORLD_W ||
        header.height != WORLD_H
    )
    {
        return false;
    }

    file.read(
        reinterpret_cast<char*>(
            world.surface.data()
        ),
        sizeof(int) *
        world.surface.size()
    );

    file.read(
        reinterpret_cast<char*>(
            world.cells.data()
        ),
        sizeof(Tile) *
        world.cells.size()
    );

    return file.good();
}

std::vector<std::string> findWorldFiles()
{
    std::vector<std::string> files;

    std::filesystem::create_directories(
        "saves"
    );

    for (
        const auto& entry :
        std::filesystem::directory_iterator(
            "saves"
        )
    )
    {
        if (!entry.is_regular_file())
            continue;

        if (
            entry.path().extension() ==
            ".oni"
        )
        {
            files.push_back(
                entry.path().string()
            );
        }
    }

    std::sort(
        files.begin(),
        files.end()
    );

    return files;
}

std::string chooseWorldFile(
    const std::vector<std::string>& files
)
{
    if (files.empty())
        return {};

    // Simple console selector for now.
    //
    // This is intentionally isolated so it can later
    // become a proper graphical load menu.

    std::printf(
        "\n==============================\n"
        " WORLD FILES\n"
        "==============================\n"
    );

    for (
        size_t i = 0;
        i < files.size();
        ++i
    )
    {
        std::printf(
            "%zu: %s\n",
            i + 1,
            files[i].c_str()
        );
    }

    std::printf(
        "Choose world number: "
    );

    int choice = 0;

    if (
        std::scanf(
            "%d",
            &choice
        ) != 1
    )
    {
        return {};
    }

    if (
        choice < 1 ||
        choice >
            static_cast<int>(files.size())
    )
    {
        return {};
    }

    return files[
        static_cast<size_t>(choice - 1)
    ];
}

}


// ============================================================
// END FILE: persistence/Save.cpp
// ============================================================
