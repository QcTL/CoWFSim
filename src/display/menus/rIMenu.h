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

    rIMenu(const std::shared_ptr<rIMenu> &mParent, rRelativePos rRelativePos, const std::string &pthFileD) :
            tsTex(gTileset("ts_menuFont8.png", 8, 40, 10)),
            rPos(rRelativePos),
            parentMenu(mParent) {
        setTransformation();

        if (!pthFileD.empty()) {
            rIMenu_objFile _dInfoFile = extractDataFromFile(pthFileD);
            dExtracted = _dInfoFile.of_data;
            comV = std::vector<defTxtCompany>(_dInfoFile.of_inLengthTex.size(), {{0, 0}});

            std::vector<uint8_t> sLengths = _dInfoFile.of_inLengthTex;
            int nSeen = 0;
            for (int i = 0; i < _dInfoFile.of_data.size(); ++i) {
                for (int j = 0; j < _dInfoFile.of_data[i].size(); ++j) {
                    auto row = (rPos == pBottomLeft || rPos == pBottomRight) ? _dInfoFile.of_data.size() - 1 - i : i;
                    auto col = (rPos == pTopRight || rPos == pBottomRight) ? _dInfoFile.of_data[i].size() - 1 - j : j;
                    if (_dInfoFile.of_data[i][j] == _dInfoFile.of_inSelTex) {
                        comV[nSeen] = {{row, col}, sLengths[nSeen]};
                        nSeen++;
                    }
                }
            }

            dInfo = getVertexMenu((int) _dInfoFile.of_data[0].size(), (int) _dInfoFile.of_data.size(),
                                  _dInfoFile.of_data);
            gWidth = (int) _dInfoFile.of_data[0].size();
            gHeight = (int) _dInfoFile.of_data.size();
        }
    }

    virtual void draw(sf::RenderWindow &rW) = 0;

    virtual void setResponse(int v, uint16_t lID) = 0;

    virtual bool interact(const sf::Event &event, const sf::RenderWindow &rWindow) = 0;

    virtual void pressedCell(std::pair<int, int> cPressed) = 0;

    virtual void update() {}

    void setTransformation() {
        auto tSize = (float)tsTex.getTileSize();
        for (int i = 0; i < 40 * 9; i++) {
            std::pair<int, int> posTopLeft = tsTex.getPos(i);
            std::vector<sf::Vector2f> tileCoords;
            switch (rPos) {
                case rIMenu::rRelativePos::pTopLeft:
                    tileCoords = {
                            sf::Vector2f((float)posTopLeft.first * tSize, (float)posTopLeft.second * tSize),
                            sf::Vector2f((float)(posTopLeft.first + 1) * tSize, (float)posTopLeft.second * tSize),
                            sf::Vector2f((float)(posTopLeft.first + 1) * tSize, (float)(posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float)posTopLeft.first * tSize, (float)(posTopLeft.second + 1) * tSize)
                    };
                    break;
                case rIMenu::rRelativePos::pBottomLeft:
                    tileCoords = {
                            sf::Vector2f((float)posTopLeft.first * tSize, (float)(posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float)(posTopLeft.first + 1) * tSize, (float)(posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float)(posTopLeft.first + 1) * tSize, (float)posTopLeft.second * tSize),
                            sf::Vector2f((float)posTopLeft.first * tSize, (float)posTopLeft.second * tSize)
                    };
                    break;
                case rIMenu::rRelativePos::pTopRight:
                    tileCoords = {
                            sf::Vector2f((float)(posTopLeft.first + 1) * tSize, (float)posTopLeft.second * tSize),
                            sf::Vector2f((float)posTopLeft.first * tSize, (float)posTopLeft.second * tSize),
                            sf::Vector2f((float)posTopLeft.first * tSize, (float)(posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float)(posTopLeft.first + 1) * tSize, (float)(posTopLeft.second + 1) * tSize)
                    };
                    break;
                case rIMenu::rRelativePos::pBottomRight:
                    tileCoords = {
                            sf::Vector2f((float)(posTopLeft.first + 1) * tSize, (float)(posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float)posTopLeft.first * tSize, (float)(posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float)posTopLeft.first * tSize, (float)posTopLeft.second * tSize),
                            sf::Vector2f((float)(posTopLeft.first + 1) * tSize, (float)posTopLeft.second * tSize)
                    };
                    break;
                default:
                    break;
            }
            lRefTiles.push_back(tileCoords);
        }
    }

    sf::VertexArray getVertexMenu(int GWidth, int GHeight, std::vector<std::vector<int>> strData) {
        float GTileSize = 16.0;
        v = sf::VertexArray(sf::Quads, GWidth * GHeight * 4);

        for (int i = 0; i < GHeight; i++) {
            for (int j = 0; j < GWidth; j++) {
                if (strData[i][j] == 0)
                    continue;
                sf::Vertex *quad;
                int xIndex, yIndex;
                switch (rPos) {
                    case rIMenu::rRelativePos::pTopLeft:
                        xIndex = j;
                        yIndex = i;
                        break;
                    case rIMenu::rRelativePos::pBottomLeft:
                        xIndex = j;
                        yIndex = GHeight - 1 - i;
                        break;
                    case rIMenu::rRelativePos::pTopRight:
                        xIndex = GWidth - 1 - j;
                        yIndex = i;
                        break;
                    case rIMenu::rRelativePos::pBottomRight:
                        xIndex = GWidth - 1 - j;
                        yIndex = GHeight - 1 - i;
                        break;
                    default:
                        xIndex = yIndex = 0;
                }
                quad = &v[(xIndex + yIndex * GWidth) * 4];
                quad[0].position = sf::Vector2f((float)xIndex * GTileSize, (float)yIndex * GTileSize);
                quad[1].position = sf::Vector2f((float)(xIndex + 1) * GTileSize, (float)yIndex * GTileSize);
                quad[2].position = sf::Vector2f((float)(xIndex + 1) * GTileSize, (float)(yIndex + 1) * GTileSize);
                quad[3].position = sf::Vector2f((float)xIndex * GTileSize, (float)(yIndex + 1) * GTileSize);

                for (int k = 0; k < 4; k++)
                    quad[k].texCoords = lRefTiles[strData[i][j]][k];
            }
        }
        return v;
    }

    rRelativePos getRPos() {
        return rPos;
    }

    sf::Vector2<uint32_t>
    getAbsPos(const sf::RenderWindow &rW, uint32_t menuH, int menuW, const sf::Vector2<int> &mouseP) {
        auto wS = rW.getSize();
        switch (rPos) {
            case pBottomLeft:
                return {(unsigned int) mouseP.x, mouseP.y - (wS.y - menuH)};
            case pBottomRight:
                return {mouseP.x - (wS.x - menuW), mouseP.y - (wS.y - menuH)};
            case pTopLeft:
                return {(unsigned int) mouseP.x, (unsigned int) mouseP.y};
            case pTopRight:
                return {mouseP.x - (wS.x - menuW), (unsigned int) mouseP.y};
            case pCenter:
            case pCenterBottom:
            case pCenterTop:
                break;
        }
        return {};
    }

    bool isInside(const sf::RenderWindow &rW, int menuH, int menuW, const sf::Vector2<int> &mouseP) {
        auto wS = rW.getSize();
        switch (rPos) {
            case pBottomLeft:
                if (mouseP.x >= 0 && mouseP.x < menuW && mouseP.y >= (wS.y - menuH) && mouseP.y < wS.y)
                    return true;
            case pBottomRight:
                if (mouseP.x >= (wS.x - menuW) && mouseP.x < wS.x && mouseP.y >= (wS.y - menuH) && mouseP.y < wS.y)
                    return true;
            case pTopLeft:
                if (mouseP.x >= 0 && mouseP.x < menuW && mouseP.y >= 0 && mouseP.y < menuH)
                    return true;
            case pTopRight:
                if (mouseP.x >= (wS.x - menuW) && mouseP.x < wS.x && mouseP.y >= 0 && mouseP.y < menuH)
                    return true;
            case pCenter:
            case pCenterBottom:
            case pCenterTop:
                break;
        }
        return false;
    }

protected:
    std::shared_ptr<rIMenu> parentMenu;
    gTileset tsTex;
    sf::VertexArray v;
    std::vector<std::vector<sf::Vector2f>> lRefTiles;
    rRelativePos rPos;

    struct defTxtCompany {
        std::pair<uint32_t, uint32_t> pStartText;
        uint8_t pLength;
    };
    std::vector<defTxtCompany> comV;

    sf::VertexArray dInfo;
    int gWidth = 0;
    int gHeight = 0;
    std::vector<std::vector<int>> dExtracted;

    struct rIMenu_objFile {
        std::vector<std::vector<int>> of_data;
        uint32_t of_inSelTex;
        std::vector<uint8_t> of_inLengthTex;
        struct of_button {
            uint32_t ofb_pX;
            uint32_t ofb_pY;
            uint32_t ofb_pHeight;
            uint32_t ofb_pWidth;
        };
        std::vector<of_button> of_vecButton;
    };

    static rIMenu_objFile extractDataFromFile(const std::string &pthFileD) {
        std::map<std::string, std::string> sm = ReaderParameters::readFile(
                (RelPath::relPath / "files" / "graphic" / "menus" / (pthFileD + R"(.txt)")).string());

        std::ifstream file((RelPath::relPath / "files" / "graphic" / "menus" / sm["src"]).string());

        std::string line;
        std::vector<std::vector<int>> _gContentF;
        while (std::getline(file, line)) {
            std::vector<int> row;
            std::istringstream iss(line);
            int value;
            while (iss >> value) {
                row.push_back(value);
            }
            _gContentF.push_back(row);
        }
        file.close();

        uint32_t _gSelTex = 0;
        if (sm.find("inSelTex") != sm.end())
            _gSelTex = std::stoul(sm["inSelTex"]);

        std::vector<uint8_t> _gVecLengthTex = {};
        if (sm.find("inLengthTex") != sm.end()) {
            std::istringstream ss(sm["inLengthTex"]);
            std::string segment;
            while (std::getline(ss, segment, ','))
                _gVecLengthTex.push_back(std::stoul(segment));
        }

        std::vector<rIMenu_objFile::of_button> _gVecPosButtons = {};
        if (sm.find("inPosButtons") != sm.end()) {
            std::istringstream ss(sm["inPosButtons"]);
            std::string segment;
            while (std::getline(ss, segment, ',')) {
                std::istringstream ssInside(sm["inPosButtons"]);
                std::string segmentInside;
                std::vector<uint32_t> _gVecComponent;
                while (std::getline(ss, segmentInside, '#'))
                    _gVecComponent.push_back(std::stoul(segmentInside));
                if (_gVecComponent.size() == 4)
                    _gVecPosButtons.push_back(
                            {_gVecComponent[0], _gVecComponent[1], _gVecComponent[2], _gVecComponent[3]});
            }
        }
        return {_gContentF, _gSelTex, _gVecLengthTex};
    }

    void setPositionValue(std::pair<int, int> cPos, uint32_t cValue) {
        sf::Vertex *quad = &dInfo[(cPos.second + cPos.first * gWidth) * 4];
        for (int k = 0; k < 4; k++)
            quad[k].texCoords = lRefTiles[cValue][k];
    }

    void setText(const uint8_t tVal, std::string cText) {
        if (rPos == rIMenu::rRelativePos::pBottomRight || rPos == rIMenu::rRelativePos::pTopRight) {
            cText.append(comV[tVal].pLength - cText.size(), ' ');
            std::reverse(cText.begin(), cText.end());
        }

        for (int i = 0; i < comV[tVal].pLength; i++) {
            sf::Vertex *quad = &dInfo[(comV[tVal].pStartText.second + i + comV[tVal].pStartText.first * gWidth) * 4];
            char currentChar = (i < cText.size()) ? cText[i] : ' ';
            int charIndex = -1;
            if (currentChar >= 'a' && currentChar <= 'z') charIndex = currentChar - 'a' + 1;
            else if (currentChar >= 'A' && currentChar <= 'Z') charIndex = currentChar - 'A' + 65;
            else if (currentChar >= '0' && currentChar <= '9') charIndex = currentChar - '0' + 48;
            else if (currentChar == '.') charIndex = 46;
            else if (currentChar == ':') charIndex = 58;
            else if (currentChar == '/') charIndex = 47;
            else if (currentChar == '-') charIndex = 45;

            const int defaultCharIndex = 32;
            for (int k = 0; k < 4; k++) {
                quad[k].texCoords = lRefTiles[(charIndex != -1) ? charIndex : defaultCharIndex][k];
            }
        }
    }

    static std::string getFloatToString2Decimal(const float nUsed) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << nUsed;
        return ss.str();
    }
};

#endif //CITYOFWEIRDFISHES_RIMENU_H
