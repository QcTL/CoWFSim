//
// Created by Laminar on 31/01/2024.
//

#ifndef CITYOFWEIRDFISHES_RIMENU_H
#define CITYOFWEIRDFISHES_RIMENU_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>
#include "../../common/RelPath.h"
#include "../layers/tileset/gTileset.h"
#include "../../IO/ReaderParameters.h"

class rIMenu {
public:
    enum rRelativePos {
        pBottomLeft, pBottomRight, pTopLeft, pTopRight, pCenter, pCenterBottom, pCenterTop
    };

    rIMenu(const std::shared_ptr<rIMenu>& mParent, rRelativePos rRelativePos) : tsTex(gTileset("ts_menuFont8.png", 8, 40, 10)),
                                        rPos(rRelativePos), parentMenu(mParent) {
        setTransformation();
    }

    virtual void draw(sf::RenderWindow &rW) = 0;
    virtual void setResponse(int v) = 0;
    virtual bool interact(const sf::Event& event, const sf::RenderWindow& rWindow) = 0;
    virtual void pressedCell(std::pair<int, int> cPressed) = 0;

    void setTransformation() {
        int tSize = tsTex.getTileSize();
        for (int i = 0; i < 40 * 9; i++) {
            std::pair<int, int> posTopLeft = tsTex.getPos(i);

            if (rPos == rIMenu::rRelativePos::pTopLeft) {
                lRefTiles.push_back(
                        std::vector<sf::Vector2f>{
                                sf::Vector2f(posTopLeft.first * tSize, posTopLeft.second * tSize),
                                sf::Vector2f((posTopLeft.first + 1) * tSize, posTopLeft.second * tSize),
                                sf::Vector2f((posTopLeft.first + 1) * tSize, (posTopLeft.second + 1) * tSize),
                                sf::Vector2f(posTopLeft.first * tSize, (posTopLeft.second + 1) * tSize)
                        }
                );
            } else if (rPos == rIMenu::rRelativePos::pBottomLeft) {
                lRefTiles.push_back(
                        std::vector<sf::Vector2f>{
                                sf::Vector2f(posTopLeft.first * tSize, (posTopLeft.second + 1) * tSize),
                                sf::Vector2f((posTopLeft.first + 1) * tSize, (posTopLeft.second + 1) * tSize),
                                sf::Vector2f((posTopLeft.first + 1) * tSize, posTopLeft.second * tSize),
                                sf::Vector2f(posTopLeft.first * tSize, posTopLeft.second * tSize)
                        }
                );
            } else if (rPos == rIMenu::rRelativePos::pTopRight){
                lRefTiles.push_back(
                        std::vector<sf::Vector2f>{
                                sf::Vector2f((posTopLeft.first + 1) * tSize, posTopLeft.second * tSize),
                                sf::Vector2f(posTopLeft.first * tSize, posTopLeft.second * tSize),
                                sf::Vector2f(posTopLeft.first * tSize, (posTopLeft.second + 1) * tSize),
                                sf::Vector2f((posTopLeft.first + 1) * tSize, (posTopLeft.second + 1) * tSize)
                        }
                );
            } else if (rPos == rIMenu::rRelativePos::pBottomRight){
                lRefTiles.push_back(
                        std::vector<sf::Vector2f>{
                                sf::Vector2f((posTopLeft.first + 1) * tSize, (posTopLeft.second + 1) * tSize),
                                sf::Vector2f(posTopLeft.first * tSize, (posTopLeft.second + 1) * tSize),
                                sf::Vector2f(posTopLeft.first * tSize, posTopLeft.second * tSize),
                                sf::Vector2f((posTopLeft.first + 1) * tSize, posTopLeft.second * tSize)
                        }
                );
            }
        }
    }

    sf::VertexArray getVertexMenu(int GWidth, int GHeight, std::vector<std::vector<int>> strData) {
        int GTileSize = 16;
        v = sf::VertexArray(sf::Quads, GWidth * GHeight * 4);
        if (rPos == rIMenu::rRelativePos::pTopLeft) {
            for (int i = 0; i < GHeight; i++) {
                for (int j = 0; j < GWidth; j++) {

                    sf::Vertex *quad = &v[(j + i * GWidth) * 4];

                    quad[0].position = sf::Vector2f(j * GTileSize, i * GTileSize);
                    quad[1].position = sf::Vector2f((j + 1) * GTileSize, i * GTileSize);
                    quad[2].position = sf::Vector2f((j + 1) * GTileSize, (i + 1) * GTileSize);
                    quad[3].position = sf::Vector2f(j * GTileSize, (i + 1) * GTileSize);

                    for (int k = 0; k < 4; k++) {
                        quad[k].texCoords = lRefTiles[strData[i][j]][k];
                    }
                }
            }
        } else if (rPos == rIMenu::rRelativePos::pBottomLeft) {
            for (int i = GHeight - 1; i >= 0; i--) {
                for (int j = 0; j < GWidth; j++) {
                    sf::Vertex *quad = &v[(j + (GHeight - 1 - i) * GWidth) * 4];

                    quad[0].position = sf::Vector2f(j * GTileSize, (GHeight - 1 - i) * GTileSize);
                    quad[1].position = sf::Vector2f((j + 1) * GTileSize, (GHeight - 1 - i) * GTileSize);
                    quad[2].position = sf::Vector2f((j + 1) * GTileSize, (GHeight - i) * GTileSize);
                    quad[3].position = sf::Vector2f(j * GTileSize, (GHeight - i) * GTileSize);

                    for (int k = 0; k < 4; k++) {
                        quad[k].texCoords = lRefTiles[strData[i][j]][k];
                    }
                }
            }
        }else if(rPos == rIMenu::rRelativePos::pTopRight){
            for (int i = 0; i < GHeight; i++) {
                for (int j = GWidth - 1; j >= 0; j--) {

                    sf::Vertex *quad = &v[((GWidth - j - 1) + i * GWidth) * 4];

                    quad[0].position = sf::Vector2f((GWidth - j - 1) * GTileSize, i * GTileSize);
                    quad[1].position = sf::Vector2f((GWidth - j) * GTileSize, i * GTileSize);
                    quad[2].position = sf::Vector2f((GWidth - j) * GTileSize, (i + 1) * GTileSize);
                    quad[3].position = sf::Vector2f((GWidth - j - 1) * GTileSize, (i + 1) * GTileSize);

                    for (int k = 0; k < 4; k++) {
                        quad[k].texCoords = lRefTiles[strData[i][j]][k];
                    }
                }
            }
        }else if (rPos == rIMenu::rRelativePos::pBottomRight){
            for (int i = GHeight - 1; i >= 0; i--) {
                for (int j = GWidth - 1; j >= 0; j--) {

                    sf::Vertex *quad = &v[((GWidth - j - 1) + (GHeight - 1 - i) * GWidth) * 4];

                    quad[0].position = sf::Vector2f((GWidth - j - 1) * GTileSize, (GHeight - 1 - i) * GTileSize);
                    quad[1].position = sf::Vector2f((GWidth - j) * GTileSize, (GHeight - 1 - i) * GTileSize);
                    quad[2].position = sf::Vector2f((GWidth - j) * GTileSize, (GHeight - i) * GTileSize);
                    quad[3].position = sf::Vector2f((GWidth - j - 1) * GTileSize, (GHeight - i) * GTileSize);

                    for (int k = 0; k < 4; k++) {
                        quad[k].texCoords = lRefTiles[strData[i][j]][k];
                    }
                }
            }
        }

        return v;
    }


    rRelativePos getRPos() {
        return rPos;
    }

    sf::Vector2<uint32_t> getAbsPos(const sf::RenderWindow& rW, uint menuH, int menuW, const sf::Vector2<int>& mouseP){
        auto wS = rW.getSize();
        switch (rPos) {
            case pBottomLeft:
                    return {(unsigned int)mouseP.x,mouseP.y - (wS.y - menuH)};
            case pBottomRight:
                    return {mouseP.x - (wS.x - menuW),mouseP.y - (wS.y - menuH) };
            case pTopLeft:
                    return {(unsigned int)mouseP.x, (unsigned int) mouseP.y };
            case pTopRight:
                    return {mouseP.x - (wS.x - menuW), (unsigned int) mouseP.y};
        }
        return {};
    }

    bool isInside(const sf::RenderWindow& rW, int menuH, int menuW, const sf::Vector2<int>& mouseP){
        auto wS = rW.getSize();
        switch (rPos) {
            case pBottomLeft:
                if(mouseP.x >= 0 && mouseP.x < menuW &&  mouseP.y >= (wS.y - menuH) && mouseP.y < wS.y)
                    return true;
            case pBottomRight:
                if(mouseP.x >= (wS.x - menuW) && mouseP.x < wS.x &&  mouseP.y >= (wS.y - menuH) && mouseP.y < wS.y)
                    return true;
            case pTopLeft:
                if(mouseP.x >= 0 && mouseP.x < menuW && mouseP.y >= 0 && mouseP.y < menuH)
                    return true;
            case pTopRight:
                if(mouseP.x >= (wS.x - menuW) && mouseP.x < wS.x && mouseP.y >= 0 && mouseP.y < menuH)
                    return true;
        }
        return false;
    }

protected:
    std::shared_ptr<rIMenu> parentMenu;
    gTileset tsTex;
    sf::VertexArray v;
    std::vector<std::vector<sf::Vector2f>> lRefTiles;
    rRelativePos rPos;


    static std::vector<std::vector<int>> extractDataFromFile(const std::string &pthFileD){
        std::map<std::string, std::string> sm = ReaderParameters::readFile(
                (RelPath::relPath / "files" / "graphic" / "menus" / (pthFileD + R"(.txt)")).string());

        std::ifstream file((RelPath::relPath / "files" / "graphic" / "menus" / sm["src"]).string());

        std::string line;
        std::vector<std::vector<int>> data;

        while (std::getline(file, line)) {
            std::vector<int> row;
            std::istringstream iss(line);
            int value;
            while (iss >> value) {
                row.push_back(value);
            }
            data.push_back(row);
        }

        file.close();
        return data;
    }
};

#endif //CITYOFWEIRDFISHES_RIMENU_H
