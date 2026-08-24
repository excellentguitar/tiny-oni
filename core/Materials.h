// ============================================================

Color materialColor(Type type)
{
    switch (type)
    {
        case Type::Grass:
            return {72, 150, 65, 255};

        case Type::Dirt:
            return {125, 82, 48, 255};

        case Type::Clay:
            return {177, 103, 67, 255};

        case Type::Claystone:
            return {145, 92, 76, 255};

        case Type::Sand:
            return {222, 194, 115, 255};

        case Type::Sandstone:
            return {194, 139, 82, 255};

        case Type::Chalk:
            return {220, 216, 195, 255};

        case Type::Limestone:
            return {188, 184, 164, 255};

        case Type::Granite:
            return {105, 112, 125, 255};

        case Type::Basalt:
            return {63, 68, 73, 255};

        case Type::Gravel:
            return {126, 124, 116, 255};

        case Type::Copper:
            return {190, 104, 58, 255};

        case Type::Iron:
            return {130, 62, 48, 255};

        case Type::Coal:
            return {38, 38, 42, 255};

        case Type::Water:
            return {45, 135, 225, 145};

        case Type::Gas:
            return {100, 190, 230, 45};

        case Type::Magma:
            return {235, 72, 22, 255};

        case Type::Wood:
            return {112, 72, 42, 255};

        case Type::Leaves:
            return {55, 125, 55, 255};

        default:
            return {18, 20, 24, 255};
    }
}

const char* materialName(Type type)
{
    switch (type)
    {
        case Type::Grass:      return "Grass";
        case Type::Dirt:       return "Dirt";
        case Type::Clay:       return "Clay";
        case Type::Claystone:  return "Claystone";
        case Type::Sand:       return "Sand";
        case Type::Sandstone:  return "Sandstone";
        case Type::Chalk:      return "Chalk";
        case Type::Limestone:  return "Limestone";
        case Type::Granite:    return "Granite";
        case Type::Basalt:     return "Basalt";
        case Type::Gravel:     return "Gravel";
        case Type::Copper:     return "Copper";
        case Type::Iron:       return "Iron";
        case Type::Coal:       return "Coal";
        case Type::Water:      return "Water";
        case Type::Gas:        return "Gas";
        case Type::Magma:      return "Magma";
        case Type::Wood:       return "Wood";
        case Type::Leaves:     return "Leaves";
        case Type::Oxygen:     return "Oxygen";
        default:               return "Empty";
    }
}


// ============================================================
// END FILE: core/Materials.h
// ============================================================
