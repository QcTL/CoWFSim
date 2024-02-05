//
// Created by Laminar on 30/01/2024.
//

#ifndef CITYOFWEIRDFISHES_GTILESET_H
#define CITYOFWEIRDFISHES_GTILESET_H

#include <string>
#include <SFML/Graphics/Texture.hpp>
#include "../../../common/RelPath.h"

class gTileset{
public:
    gTileset(std::string pthTS, int nPixelsSize, int tWidth, int tHeight):
        tsNWidth(tWidth), tsNHeight(tHeight), tsSize(nPixelsSize){
        //if (!tsTex.loadFromFile(R"(C:\Users\Robert\CityOfWeirdFishes\files\graphic\tilesets\)" + pthTS)) {
        if (!tsTex.loadFromFile((RelPath::relPath / "files" / "graphic" / "tilesets" / pthTS).string())){
            std::cout << "WARNING: A TEXTURE HAS NOT LOADED PROPERLY" << std::endl;
        }
    }

    std::pair<int,int> getPos(int i){
        return {i%tsNWidth, i/tsNWidth};
    }

    int getTileSize() const{
        return tsSize;
    }
    sf::Texture tsTex;

private:

    int tsNWidth;
    int tsNHeight;
    int tsSize;
};

#endif //CITYOFWEIRDFISHES_GTILESET_H
