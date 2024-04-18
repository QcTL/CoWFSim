//
// Created by ganymede on 2/23/24.
//

#ifndef CITYOFWEIRDFISHES_GLAYERTRANSIT_H
#define CITYOFWEIRDFISHES_GLAYERTRANSIT_H

#include "../gILayer.h"

class gLayerTransit : public gILayer {
public:
    explicit gLayerTransit(const std::shared_ptr<gIGrid<uint8_t>> &gGrid,
                           const std::shared_ptr<gIGrid<uint32_t>> &gCityGrid) :
            gILayer(gTileset("ts_transit16.png", 16, 10, 40)), lGrid(gGrid), lCityGrid(gCityGrid) {
        //setTransformation({0, 1, 2, 3, 4, 5,6,7,8,9,10,11,12,13,14});

        addToMapSprites(0, getByPosTopLeft(lTs.getPos(42)));
        addRoadToMapSprites();
    }

    std::vector<sf::Vector2f> getTexPos(int x, int y) override {
        //return rIM_lRefTiles[lGrid->get(x, y)];

        uint32_t v = lCityGrid->get(x, y);
        auto vType = static_cast<uint8_t>(v >> 24);
        if (vType != 32 && vType != 33)
            return mTypeSprites.at(0)[0];
        return mTypeSprites.at((vType == 32 ? 1 : 10) + (lGrid->get(x, y) - 1)) [v & 0xFF];
    }

    void addRoadToMapSprites() {
        int rRotate[] = {11, 1, 0, 10};
        std::vector<int> rTopLeftPos = {66, 46, 26, 8, 28, 64, 60, 40, 20, 0, 6, 62, 48, 48, 04, 02, 22, 24, 44, 80};

        for (int tra = 0; tra < 4; tra++) {
            for (uint32_t i = 0; i < rTopLeftPos.size(); i++) {
                uint32_t cTypeRoad = r2BitDirection::getTypeRoadById(i);
                for (int j: rRotate) {
                    addToMapSprites((cTypeRoad == 1 ? 1 : 10) + tra,
                                    getByPosTopLeft(lTs.getPos(rTopLeftPos[i] + 100*tra + j)));
                }
            }
        }
    }

    void addToMapSprites(const uint8_t &n, const std::vector<sf::Vector2f> &v) {
        if (mTypeSprites.find(n) == mTypeSprites.end()) {
            mTypeSprites[n] = std::vector<std::vector<sf::Vector2f>>();
        }
        mTypeSprites[n].push_back(v);
    }

    std::vector<sf::Vector2f> getByPosTopLeft(const std::pair<int, int> posTopLeft) {
        int tSize = lTs.getTileSize();
        return std::vector<sf::Vector2f>{
                sf::Vector2f(posTopLeft.first * tSize, posTopLeft.second * tSize),
                sf::Vector2f((posTopLeft.first + 1) * tSize, posTopLeft.second * tSize),
                sf::Vector2f((posTopLeft.first + 1) * tSize, (posTopLeft.second + 1) * tSize),
                sf::Vector2f(posTopLeft.first * tSize, (posTopLeft.second + 1) * tSize)
        };
    }

private:
    std::shared_ptr<gIGrid<uint8_t>> lGrid;
    std::shared_ptr<gIGrid<uint32_t>> lCityGrid;

    std::map<uint8_t, std::vector<std::vector<sf::Vector2f>>> mTypeSprites;
};

#endif //CITYOFWEIRDFISHES_GLAYERTRANSIT_H
