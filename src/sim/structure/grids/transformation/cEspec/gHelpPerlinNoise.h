//
// Created by Laminar on 29/02/2024.
//

#ifndef CITYOFWEIRDFISHES_GHELPPERLINNOISE_H
#define CITYOFWEIRDFISHES_GHELPPERLINNOISE_H

#include <cmath>

class PerlinNoise{
private:
    float fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    float grad(int hash, float x) {
        int h = hash & 15;
        float grad = 1.0f + (h & 7);
        if (h & 8) grad = -grad;
        return grad * x;
    }
public:
    float perlinNoise(float x, float y) {
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;

        x -= std::floor(x);
        y -= std::floor(y);

        float u = fade(x);
        float v = fade(y);

        int A = 0, B = 1;
        int AA = (A + X) & 255;
        int BA = (B + X) & 255;
        int AB = (A + Y) & 255;
        int BB = (B + Y) & 255;

        float gradAA = grad(AA, x);
        float gradBA = grad(BA, x);
        float gradAB = grad(AB, y);
        float gradBB = grad(BB, y);

        float x1 = lerp(gradAA, gradBA, u);
        float x2 = lerp(gradAB, gradBB, u);

        return lerp(x1, x2, v);
    }
};

#endif //CITYOFWEIRDFISHES_GHELPPERLINNOISE_H
