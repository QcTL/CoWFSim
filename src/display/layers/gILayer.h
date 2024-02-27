//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GILAYER_H
#define CITYOFWEIRDFISHES_GILAYER_H

#include <memory>
#include <utility>
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "../../sim/structure/grids/gIGrid.h"
#include "tileset/gTileset.h"


class gILayer {
public:

    explicit gILayer(gTileset gTS) : lTs(std::move(gTS)) {}

    virtual ~gILayer() = default;

    void setTransformation(const std::vector<int> &lTrans) {
        int tSize = lTs.getTileSize();
        for (auto &trans: lTrans) {
            std::pair<int, int> posTopLeft = lTs.getPos(trans);
            lRefTiles.push_back(
                    std::vector<sf::Vector2f>{
                            sf::Vector2f(posTopLeft.first * tSize, posTopLeft.second * tSize),
                            sf::Vector2f((posTopLeft.first + 1) * tSize, posTopLeft.second * tSize),
                            sf::Vector2f((posTopLeft.first + 1) * tSize, (posTopLeft.second + 1) * tSize),
                            sf::Vector2f(posTopLeft.first * tSize, (posTopLeft.second + 1) * tSize)
                    }
            );
        }
    }


    virtual std::vector<sf::Vector2f> getTexPos(int x, int y) = 0;

    std::vector<sf::Vector2f> getTexPos(std::pair<int, int> pos) {
        return getTexPos(pos.first, pos.second);
    };

    sf::Texture *getTexture() {
        return &lTs.tsTex;
    }

protected:
    gTileset lTs;
    std::vector<std::vector<sf::Vector2f>> lRefTiles;

};

#endif //CITYOFWEIRDFISHES_GILAYER_H
