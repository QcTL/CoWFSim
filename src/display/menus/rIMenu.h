//
// Created by Laminar on 31/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RIMENU_H
#define CITYOFWEIRDFISHES_RIMENU_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <SFML/Graphics/VertexArray.hpp>
#include "../../common/RelPath.h"
#include "../layers/tileset/gTileset.h"

class rIMenu {
public:
    rIMenu() : tsTex(gTileset("ts_menuFont8.png", 8, 40, 10)) {
        setTransformation();
    }

    virtual void draw(sf::RenderWindow &rW) = 0;

    void setTransformation() {
        int tSize = tsTex.getTileSize();
        for (int i = 0; i < 40 * 9; i++) {
            std::pair<int, int> posTopLeft = tsTex.getPos(i);
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

    sf::VertexArray getVertexMenu(int GWidth, int GHeight, std::vector<std::vector<int>> strData) {
        int GTileSize = 16;
        v = sf::VertexArray(sf::Quads, GWidth * GHeight * 4);
        for (int i = 0; i < GHeight; i++) {
            for (int j = 0; j < GWidth; j++) {

                sf::Vertex* quad = &v[(j + i * GWidth) * 4];

                quad[0].position = sf::Vector2f(j * GTileSize, i * GTileSize);
                quad[1].position = sf::Vector2f((j + 1) * GTileSize, i * GTileSize);
                quad[2].position = sf::Vector2f((j + 1) * GTileSize, (i + 1) * GTileSize);
                quad[3].position = sf::Vector2f(j * GTileSize, (i + 1) * GTileSize);

                for (int k = 0; k < 4; k++) {
                    quad[k].texCoords = lRefTiles[strData[i][j]][k];
                }

            }
        }
        return v;
    }

protected:
    gTileset tsTex;
    sf::VertexArray v;
    std::vector<std::vector<sf::Vector2f>> lRefTiles;
};

#endif //CITYOFWEIRDFISHES_RIMENU_H
