//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GTILESET_H
#define CITYOFWEIRDFISHES_GTILESET_H

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include "../../../common/RelPath.h"

/**
 * @class gTileset
 * @brief This class contain the information of the file that will be used as a tileset for a graphical layer
 */
class gTileset {
public:
    gTileset(const std::string &pthTS, int nPixelsSize, int tWidth, int tHeight) :
            tsNWidth(tWidth), tsNHeight(tHeight), tsSize(nPixelsSize) {
        if (!tsTex.loadFromFile((RelPath::relPath / "files" / "graphic" / "tilesets" / pthTS).string())) {
            std::cout << "WARNING: A TEXTURE HAS NOT LOADED PROPERLY" << std::endl;
        }
    }

    /**
     * @fn std::pair<int,int> getPos
     * @brief Given a index of a sprite returns the real coordinate where its placed on the file
     * @param i Index of the sprite, cannot be so  large that will reference ones outside the file
     * @return The real pair coordinates of that sprite lubricated on the file
     */
    std::pair<int, int> getPos(int i) const {
        return {i % tsNWidth, i / tsNWidth};
    }

    /**
     * @fn int getTileSize()
     * @return Returns the size of the square tiles present on the file
     */
    int getTileSize() const {
        return tsSize;
    }

    sf::Texture tsTex;

private:
    int tsNWidth;
    int tsNHeight;
    int tsSize;
};

#endif //CITYOFWEIRDFISHES_GTILESET_H
