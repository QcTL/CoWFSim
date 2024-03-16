//
// Created by Laminar on 14/03/2024.
//

#ifndef CITYOFWEIRDFISHES_GLAYERUNDERGROUND_H
#define CITYOFWEIRDFISHES_GLAYERUNDERGROUND_H

#include <utility>

#include "../gILayer.h"

class gLayerUnderground : public gILayer {
public:
    explicit gLayerUnderground(const std::shared_ptr<gIGrid<uint8_t>> &gGrid)
            : gILayer(gTileset("ts_underground16.png", 16, 7, 4)), lGrid(gGrid) {

        std::vector<int> rTopLeftPos = {7, 1, 0, 10, 1, 1, 3, 12, 0, 9, 0, 5, 2, 6, 13, 4};
        for (const int t: rTopLeftPos) {
            gPosSprites.first.push_back(getByPosTopLeft(lTs.getPos(t)));
            gPosSprites.second.push_back(getByPosTopLeft(lTs.getPos(t + 14)));
        }

    }

    std::vector<sf::Vector2f> getTexPos(int x, int y) override {
        uint32_t v = lGrid->get(x, y);
        uint8_t vSpec = v & 0xF;
        return  static_cast<uint8_t>(v >> 4) ? gPosSprites.second[vSpec] : gPosSprites.first[vSpec];
    }

    std::shared_ptr<gIGrid<uint8_t>> lGrid;
    std::pair<std::vector<std::vector<sf::Vector2f>>, std::vector<std::vector<sf::Vector2f>>> gPosSprites;

};

#endif //CITYOFWEIRDFISHES_GLAYERUNDERGROUND_H
