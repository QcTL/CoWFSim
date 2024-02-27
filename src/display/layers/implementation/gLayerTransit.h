//
// Created by ganymede on 2/23/24.
//

#ifndef CITYOFWEIRDFISHES_GLAYERTRANSIT_H
#define CITYOFWEIRDFISHES_GLAYERTRANSIT_H

#include "../gILayer.h"

class gLayerTransit : public gILayer {
public:
    explicit gLayerTransit(const std::shared_ptr<gIGrid<uint8_t>> &gGrid) :
            gILayer(gTileset("ts_pollution16.png", 16, 6, 2)), lGrid(gGrid) {
        setTransformation({0, 1, 2, 3, 4, 5});
    }

    std::vector<sf::Vector2f> getTexPos(int x, int y) override {
        return lRefTiles[lGrid->get(x, y)];
    }

private:
    std::shared_ptr<gIGrid<uint8_t>> lGrid;
};

#endif //CITYOFWEIRDFISHES_GLAYERTRANSIT_H
