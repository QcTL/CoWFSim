//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GLAYERCITY_H
#define CITYOFWEIRDFISHES_GLAYERCITY_H

#include <utility>
#include <map>

#include "../gILayer.h"

class gLayerCity : public gILayer {
public:
    explicit gLayerCity(const std::shared_ptr<gIGrid<uint32_t>> &gGrid)
    : gILayer(gTileset("ts_city16.png", 16, 10, 10)), lGrid(gGrid) {
        getInfoBlocks();
    }

    std::vector<sf::Vector2f> getTexPos(int x, int y) override {
        uint32_t v = lGrid->get(x, y);
        uint8_t vType = static_cast<uint8_t>(v >> 24);
        uint8_t vSpec = v & 0xFF;
        //return mTypeSprites.at(vType)[(x+y + x*2+y/3)% mTypeSprites.at(vType).size()];
        return mTypeSprites.at(vType)[vSpec];
    }
private:
    std::shared_ptr<gIGrid<uint32_t>> lGrid;

    void getInfoBlocks() {
        // Define the positions for each sprite
        int carreteres[] = {23, 39, 38, 35, 29, 23, 25, 37, 28, 34, 33, 36, 24, 27, 26, 32};

        // Empty
        addToMapSprites(0, getByPosTopLeft(lTs.getPos(5)));

        // Houses
        addToMapSprites(16, getByPosTopLeft(lTs.getPos(0)));
        addToMapSprites(16, getByPosTopLeft(lTs.getPos(1)));
        addToMapSprites(16, getByPosTopLeft(lTs.getPos(2)));
        addToMapSprites(16, getByPosTopLeft(lTs.getPos(3)));
        addToMapSprites(16, getByPosTopLeft(lTs.getPos(4)));
        addToMapSprites(17, getByPosTopLeft(lTs.getPos(10)));
        addToMapSprites(17, getByPosTopLeft(lTs.getPos(11)));
        addToMapSprites(18, getByPosTopLeft(lTs.getPos(20)));
        addToMapSprites(18, getByPosTopLeft(lTs.getPos(21)));
        addToMapSprites(18, getByPosTopLeft(lTs.getPos(22)));
        addToMapSprites(19, getByPosTopLeft(lTs.getPos(30)));
        addToMapSprites(19, getByPosTopLeft(lTs.getPos(31)));
        addToMapSprites(20, getByPosTopLeft(lTs.getPos(40)));
        addToMapSprites(20, getByPosTopLeft(lTs.getPos(41)));

        // Carreteres
        for (int i = 0; i < sizeof(carreteres)/sizeof(carreteres[0]); i++) {
            addToMapSprites(32, getByPosTopLeft(lTs.getPos(carreteres[i])));
        }

        for (int i = 0; i < sizeof(carreteres)/sizeof(carreteres[0]); i++) {
            addToMapSprites(33, getByPosTopLeft(lTs.getPos(carreteres[i] + 20)));
        }
    }

    void addToMapSprites(const uint8_t& n, const std::vector<sf::Vector2f>& v){
        if(mTypeSprites.find(n) == mTypeSprites.end()) {
            mTypeSprites[n] = std::vector<std::vector<sf::Vector2f>>();
        }
        mTypeSprites[n].push_back(v);
    }

    std::vector<sf::Vector2f> getByPosTopLeft(const std::pair<int,int> posTopLeft){
        int tSize = lTs.getTileSize();
        return std::vector<sf::Vector2f>{
                sf::Vector2f(posTopLeft.first * tSize, posTopLeft.second * tSize),
                sf::Vector2f((posTopLeft.first + 1) * tSize, posTopLeft.second * tSize),
                sf::Vector2f((posTopLeft.first + 1) * tSize, (posTopLeft.second + 1) * tSize),
                sf::Vector2f(posTopLeft.first * tSize, (posTopLeft.second + 1) * tSize)
        };
    }

    std::map<uint8_t, std::vector<std::vector<sf::Vector2f>>> mTypeSprites;

};

#endif //CITYOFWEIRDFISHES_GLAYERCITY_H
