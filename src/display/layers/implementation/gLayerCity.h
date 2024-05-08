//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GLAYERCITY_H
#define CITYOFWEIRDFISHES_GLAYERCITY_H

#include <utility>
#include <map>

#include "../gILayer.h"
#include "../../../common/r2BitDirection.h"

/**
 * @class gLayerCity
 * @brief This class implement the graphical layer to see a graphical representation of the simulated city
 */
class gLayerCity : public gILayer {
public:
    explicit gLayerCity(const std::shared_ptr<gIGrid<uint32_t>> &gGrid)
            : gILayer(gTileset("ts_city16.png", 16, 10, 19)), lGrid(gGrid) {
        getInfoBlocks();
    }

    std::vector<sf::Vector2f> getTexPos(int x, int y) override {
        uint32_t v = lGrid->get(x, y);
        auto vType = static_cast<uint8_t>(v >> 24);
        uint8_t vSpec = v & 0xFF;
        if(vType == 0)
            return  mTypeSprites.at(vType)[0];
        return mTypeSprites.at(vType)[vSpec];
    }

private:
    std::shared_ptr<gIGrid<uint32_t>> lGrid;

    /**
     * @fn void getInfoBlocks
     * @brief This function has to be called to load the elements necessary for the displacement on a map ordered by the type.
     */
    void getInfoBlocks() {
        // Empty
        addToMapSprites(0, getByPosTopLeft(lTs.getPos(42)));

        // Houses
        int sHType1[] = {130, 131, 132, 133};
        int sHType2[] = {140, 141,142};
        int sHType3[] = {150, 151, 152, 153};
        int sHType4[] = {160, 161};
        int sHType5[] = {170, 171, 172};

        for (const int t: sHType1)
            addToMapSprites(16, getByPosTopLeft(lTs.getPos(t)));
        for (const int t: sHType2)
            addToMapSprites(17, getByPosTopLeft(lTs.getPos(t)));
        for (const int t: sHType3)
            addToMapSprites(18, getByPosTopLeft(lTs.getPos(t)));
        for (const int t: sHType4)
            addToMapSprites(19, getByPosTopLeft(lTs.getPos(t)));
        for (const int t: sHType5)
            addToMapSprites(21, getByPosTopLeft(lTs.getPos(t)));

        // Carreteres
        addRoadToMapSprites();

        // Fields:
        int s4Way[] = {22, 39, 38, 35, 29, 23, 25, 37, 28, 34, 33, 36, 24, 27, 26, 32};

        for (const int t: s4Way)
            addToMapSprites(64, getByPosTopLeft(lTs.getPos(t + 60)));

        //Water:
        int wTiles[] = {85, 75, 95, 84, 86, 74, 94, 96, 76, 83, 82, 72, 73};

        for (const int t: wTiles)
            addToMapSprites(48, getByPosTopLeft(lTs.getPos(t + 30)));

    }

    /**
     * @fn   void addRoadToMapSprites
     * @brief Add the different types of roads and its rotations to the valid sprites that can be selected
     */
    void addRoadToMapSprites() {
        int rRotate[] = {11,1,0,10};
        std::vector<int> rTopLeftPos = {66, 46, 26, 8, 28, 64, 60, 40, 20, 0, 6, 62, 48, 48, 04, 02, 22, 24, 44,80};

        for (uint32_t i = 0; i < rTopLeftPos.size(); i++) {
            uint32_t cTypeRoad = r2BitDirection::getTypeRoadById(i);

            for (int j: rRotate) { // ONE FOR EACH ROTATION
                addToMapSprites(cTypeRoad == 1 ? 32 : 33,
                                getByPosTopLeft(lTs.getPos(rTopLeftPos[i] + j)));
            }
        }
    }

    /**
     * @fn void addToMapSprites
     * @brief Add element to the structure
     * @param n The type of element that you are trying to add an element to
     * @param v The element added indexed by its position on creation
     */
    void addToMapSprites(const uint8_t &n, const std::vector<sf::Vector2f> &v) {
        if (mTypeSprites.find(n) == mTypeSprites.end()) {
            mTypeSprites[n] = std::vector<std::vector<sf::Vector2f>>();
        }
        mTypeSprites[n].push_back(v);
    }

    std::map<uint8_t, std::vector<std::vector<sf::Vector2f>>> mTypeSprites;
};

#endif //CITYOFWEIRDFISHES_GLAYERCITY_H
