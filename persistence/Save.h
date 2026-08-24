#pragma once

#include <string>
#include <vector>

class World;

namespace SaveSystem
{
    bool saveWorld(
        const World& world,
        const std::string& filename
    );

    bool loadWorld(
        World& world,
        const std::string& filename
    );

    std::vector<std::string> findWorldFiles();

    std::string chooseWorldFile(
        const std::vector<std::string>& files
    );
}