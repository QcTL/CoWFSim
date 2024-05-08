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

/**
 * @class gILayer
 * @brief The Interface the classes has to comply to be a valid displayed graphical layer
 */
class gILayer {
public:

    explicit gILayer(gTileset gTS) : lTs(std::move(gTS)) {}

    virtual ~gILayer() = default;

    /**
     * @brief If the elements index of the displayed sprites are consecutive and correlated with the number of index,
     * fill the mapped values index <- sprite with a concrete order on the sprites
     * @param lTrans A list of positive integers containing the order of appearance of the sprites in the file
     */
    void setBasicTransformation(const std::vector<int> &lTrans) {
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

    /**
     * @fn std::vector<sf::Vector2f> getTexPos
     * @brief Returns the sprite coordinates and vertices of the texture of a given point in the layer
     * @param x The x coordinate of the position you want to get the sprite  0 < x < sizeLayer.width
     * @param y The y coordinate of the position you want to get the sprite  0 < y < sizeLayer.width
     * @return The vertex coordinates of the texture that has to be filled for that vertex
     */
    virtual std::vector<sf::Vector2f> getTexPos(int x, int y) = 0;

    /**
     * @fn sf::Texture *getTexture
     * @brief Returns the texture that the gILayer has assigned to it
     * @return A sf::Texture containing the texture information that determines the way its represented
     */
    sf::Texture *getTexture() {
        return &lTs.tsTex;
    }

    /**
    * @fn std::vector<sf::Vector2f> getByPosTopLeft
    * @param posTopLeft the top left coordinates of the square polygon you want to draw
    * @return The vector of coordinates of the rectangle to display.
    */
    virtual std::vector<sf::Vector2f> getByPosTopLeft(const std::pair<int, int> posTopLeft) {
        int tSize = lTs.getTileSize();
        return std::vector<sf::Vector2f>{
                sf::Vector2f(posTopLeft.first * tSize, posTopLeft.second * tSize),
                sf::Vector2f((posTopLeft.first + 1) * tSize, posTopLeft.second * tSize),
                sf::Vector2f((posTopLeft.first + 1) * tSize, (posTopLeft.second + 1) * tSize),
                sf::Vector2f(posTopLeft.first * tSize, (posTopLeft.second + 1) * tSize)
        };
    }

protected:
    gTileset lTs;
    std::vector<std::vector<sf::Vector2f>> lRefTiles;
    //With this variable you can map the indexes to the vertex positions the index represents in form of a sprite

};

#endif //CITYOFWEIRDFISHES_GILAYER_H
