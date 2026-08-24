// ============================================================

struct RGB
{
    float r;
    float g;
    float b;
};

RGB blend(RGB a, RGB b, float t);

Color rgbColor(RGB c);

RGB skyAtTime(float timeOfDay);

void drawSky(
    int screenWidth,
    int screenHeight,
    float timeOfDay
);


// ============================================================
// END FILE: rendering/Sky.h
// ============================================================
