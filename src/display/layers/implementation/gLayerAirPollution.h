//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GLAYERAIRPOLLUTION_H
#define CITYOFWEIRDFISHES_GLAYERAIRPOLLUTION_H

#include <utility>

#include "../gILayer.h"

class gLayerAirPollution : public gILayer {
public:
    explicit gLayerAirPollution(const std::shared_ptr<gIGrid<uint8_t>>& gGrid)
    : gILayer(gTileset("ts_pollution16.png", 16, 6, 2)), lGrid(gGrid) {
        setTransformation({0, 1, 2, 3, 4, 5,6,7,8,9,10,11});
    }

    std::vector<sf::Vector2f> getTexPos(int x, int y) override{
        return lRefTiles[lGrid->get(x,y)];
    }
    std::shared_ptr<gIGrid<uint8_t>> lGrid;
};

#endif //CITYOFWEIRDFISHES_GLAYERAIRPOLLUTION_H
