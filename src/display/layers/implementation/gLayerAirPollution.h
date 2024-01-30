//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GLAYERAIRPOLLUTION_H
#define CITYOFWEIRDFISHES_GLAYERAIRPOLLUTION_H

#include <utility>

#include "../gILayer.h"

class gLayerAirPollution : public gILayer {
public:
    explicit gLayerAirPollution( std::shared_ptr<gIGrid<int>> gGrid) : gILayer(gTileset("ts_pollution16.png", 16, 6, 2), std::move(gGrid)) {}

    std::vector<sf::Vector2f> getTexPos(int x, int y) override{
        return lRefTiles[lGrid->get(x,y)];
    }

};

#endif //CITYOFWEIRDFISHES_GLAYERAIRPOLLUTION_H
