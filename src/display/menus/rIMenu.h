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
#include <cmath>
#include "../../common/RelPath.h"
#include "../layers/tileset/gTileset.h"
#include "../../IO/ReaderParameters.h"

class rIMenu {
public:
    enum rRelativePos {
        pBottomLeft, pBottomRight, pTopLeft, pTopRight, pCenter, pCenterBottom, pCenterTop
    };

    rIMenu(const std::shared_ptr<rIMenu> &mParent, rRelativePos rRelativePos, const std::string &pthFileD) :
            rIM_tsTex(gTileset("ts_menuFont8.png", 8, 40, 10)),
            rIM_rPos(rRelativePos),
            rIM_parentMenu(mParent) {
        setTransformation();

        if (!pthFileD.empty()) {
            rIMenu_objFile _dInfoFile = extractDataFromFile(pthFileD);
            rIM_dExtracted = _dInfoFile.of_data;
            rIM_comV = std::vector<defTxtCompany>(_dInfoFile.of_inLengthTex.size(), {{0, 0}});
            rIM_comVButtons = _dInfoFile.of_vecButton;
            rIM_comVEyes = _dInfoFile.of_vecEye;
            rIM_comVEyesState = std::vector<bool>(_dInfoFile.of_vecEye.size(), false);

            std::vector<uint8_t> sLengths = _dInfoFile.of_inLengthTex;
            int nSeen = 0;
            for (int i = 0; i < _dInfoFile.of_data.size(); ++i) {
                for (int j = 0; j < _dInfoFile.of_data[i].size(); ++j) {
                    auto row = (rIM_rPos == pBottomLeft || rIM_rPos == pBottomRight) ? _dInfoFile.of_data.size() - 1 - i
                                                                                     : i;
                    auto col = (rIM_rPos == pTopRight || rIM_rPos == pBottomRight) ? _dInfoFile.of_data[i].size() - 1 -
                                                                                     j : j;
                    if (_dInfoFile.of_data[i][j] == _dInfoFile.of_inSelTex) {
                        rIM_comV[nSeen] = {{row, col}, sLengths[nSeen]};
                        nSeen++;
                    }
                }
            }

            rIM_dInfo = getVertexMenu((int) _dInfoFile.of_data[0].size(), (int) _dInfoFile.of_data.size(),
                                      _dInfoFile.of_data);
            rIM_gWidth = (int) _dInfoFile.of_data[0].size();
            rIM_gHeight = (int) _dInfoFile.of_data.size();
        }
    }

    virtual void setEyeVisualValue(uint32_t inUuidEye, bool inNewStateValue) {
        sf::Vertex *_quad;
        switch (rIM_rPos) {
            case pBottomLeft:
                _quad = &rIM_dInfo[(rIM_comVEyes[inUuidEye].ofb_pX +
                                    (rIM_gHeight - 1 - rIM_comVEyes[inUuidEye].ofb_pY) * rIM_gWidth) * 4];
                break;
            case pBottomRight:
                _quad = &rIM_dInfo[(rIM_gWidth - 1 - rIM_comVEyes[inUuidEye].ofb_pX +
                                    (rIM_gHeight - 1 - rIM_comVEyes[inUuidEye].ofb_pY) * rIM_gWidth) *
                                   4];
                break;
            case pTopLeft:
                _quad = &rIM_dInfo[(rIM_comVEyes[inUuidEye].ofb_pX + (rIM_comVEyes[inUuidEye].ofb_pY) * rIM_gWidth) *
                                   4];
                break;
            case pTopRight:
                _quad = &rIM_dInfo[(rIM_gWidth - 1 - rIM_comVEyes[inUuidEye].ofb_pX +
                                    (rIM_comVEyes[inUuidEye].ofb_pY) * rIM_gWidth) * 4];
                break;
            default:
                break;
        }

        for (int k = 0; k < 4; k++)
            _quad[k].texCoords = rIM_lRefTiles[inNewStateValue ? 277 : 279][k];
    }

    virtual void draw(sf::RenderWindow &inRenderWin) = 0;

    virtual void setResponse(int inValResponse, uint16_t inLIDSender) = 0;

    virtual bool interact(const sf::Event &inEvent, const sf::RenderWindow &inRenderWin) = 0;

    virtual void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inCDate) = 0;

    virtual void update() {}

    void setTransformation() {
        auto tSize = (float) rIM_tsTex.getTileSize();
        for (int i = 0; i < 40 * 9; i++) {
            std::pair<int, int> posTopLeft = rIM_tsTex.getPos(i);
            std::vector<sf::Vector2f> tileCoords;
            switch (rIM_rPos) {
                case rIMenu::rRelativePos::pTopLeft:
                    tileCoords = {
                            sf::Vector2f((float) posTopLeft.first * tSize, (float) posTopLeft.second * tSize),
                            sf::Vector2f((float) (posTopLeft.first + 1) * tSize, (float) posTopLeft.second * tSize),
                            sf::Vector2f((float) (posTopLeft.first + 1) * tSize,
                                         (float) (posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float) posTopLeft.first * tSize, (float) (posTopLeft.second + 1) * tSize)
                    };
                    break;
                case rIMenu::rRelativePos::pBottomLeft:
                    tileCoords = {
                            sf::Vector2f((float) posTopLeft.first * tSize, (float) (posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float) (posTopLeft.first + 1) * tSize,
                                         (float) (posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float) (posTopLeft.first + 1) * tSize, (float) posTopLeft.second * tSize),
                            sf::Vector2f((float) posTopLeft.first * tSize, (float) posTopLeft.second * tSize)
                    };
                    break;
                case rIMenu::rRelativePos::pTopRight:
                    tileCoords = {
                            sf::Vector2f((float) (posTopLeft.first + 1) * tSize, (float) posTopLeft.second * tSize),
                            sf::Vector2f((float) posTopLeft.first * tSize, (float) posTopLeft.second * tSize),
                            sf::Vector2f((float) posTopLeft.first * tSize, (float) (posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float) (posTopLeft.first + 1) * tSize,
                                         (float) (posTopLeft.second + 1) * tSize)
                    };
                    break;
                case rIMenu::rRelativePos::pBottomRight:
                    tileCoords = {
                            sf::Vector2f((float) (posTopLeft.first + 1) * tSize,
                                         (float) (posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float) posTopLeft.first * tSize, (float) (posTopLeft.second + 1) * tSize),
                            sf::Vector2f((float) posTopLeft.first * tSize, (float) posTopLeft.second * tSize),
                            sf::Vector2f((float) (posTopLeft.first + 1) * tSize, (float) posTopLeft.second * tSize)
                    };
                    break;
                default:
                    break;
            }
            rIM_lRefTiles.push_back(tileCoords);
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
                switch (rIM_rPos) {
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
                quad[0].position = sf::Vector2f((float) xIndex * GTileSize, (float) yIndex * GTileSize);
                quad[1].position = sf::Vector2f((float) (xIndex + 1) * GTileSize, (float) yIndex * GTileSize);
                quad[2].position = sf::Vector2f((float) (xIndex + 1) * GTileSize, (float) (yIndex + 1) * GTileSize);
                quad[3].position = sf::Vector2f((float) xIndex * GTileSize, (float) (yIndex + 1) * GTileSize);

                for (int k = 0; k < 4; k++)
                    quad[k].texCoords = rIM_lRefTiles[strData[i][j]][k];
            }
        }
        return v;
    }

    rRelativePos getRPos() {
        return rIM_rPos;
    }

    sf::Vector2<uint32_t>
    getAbsPos(const sf::RenderWindow &rW, uint32_t menuH, int menuW, const sf::Vector2<int> &mouseP) {
        auto wS = rW.getSize();
        switch (rIM_rPos) {
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


    sf::Vector2<int> getRelPosMenu(const sf::RenderWindow &inRenderWin, const sf::Vector2<int> &inMousePos) {
        sf::Vector2<int> _mouseRel = {inMousePos.x / 16, inMousePos.y / 16};
        sf::Vector2<int> _winSize = {(int) inRenderWin.getSize().x / 16, (int) inRenderWin.getSize().y / 16};

        if (rIM_rPos == rRelativePos::pBottomRight || rIM_rPos == rRelativePos::pTopRight)
            _mouseRel.x = _mouseRel.x - (_winSize.x - rIM_gWidth);

        if (rIM_rPos == rRelativePos::pBottomRight || rIM_rPos == rRelativePos::pBottomLeft)
            _mouseRel.y = _mouseRel.y - (_winSize.y - rIM_gHeight) - 1;

        return _mouseRel;
    }

    int getButtonPressed(const sf::RenderWindow &inRenderWin, const sf::Vector2<int> &inMousePos) {
        sf::Vector2<int> _mouseRel = getRelPosMenu(inRenderWin, inMousePos);
        for (int i = 0; i < rIM_comVButtons.size(); i++)
            if (_mouseRel.x >= rIM_comVButtons[i].ofb_pX && _mouseRel.y >= rIM_comVButtons[i].ofb_pY &&
                _mouseRel.x <= (rIM_comVButtons[i].ofb_pX + rIM_comVButtons[i].ofb_pWidth) &&
                _mouseRel.y <= (rIM_comVButtons[i].ofb_pY + rIM_comVButtons[i].ofb_pHeight))
                return i;
        return -1;
    }

    int getEyePressed(const sf::RenderWindow &inRenderWin, const sf::Vector2<int> &inMousePos) {
        sf::Vector2<int> _mouseRel = getRelPosMenu(inRenderWin, inMousePos);
        for (int i = 0; i < rIM_comVEyes.size(); i++)
            if (_mouseRel.x >= rIM_comVEyes[i].ofb_pX && _mouseRel.y >= rIM_comVEyes[i].ofb_pY &&
                _mouseRel.x <= (rIM_comVEyes[i].ofb_pX + rIM_comVEyes[i].ofb_pWidth) &&
                _mouseRel.y <= (rIM_comVEyes[i].ofb_pY + rIM_comVEyes[i].ofb_pHeight))
                return i;
        return -1;
    }

    bool isInside(const sf::RenderWindow &inRenderWin, int inMenuH, int inMenuW, const sf::Vector2<int> &inMousePos) {
        auto _wS = inRenderWin.getSize();
        switch (rIM_rPos) {
            case pBottomLeft:
                if (inMousePos.x >= 0 && inMousePos.x < inMenuW && inMousePos.y >= (_wS.y - inMenuH) &&
                    inMousePos.y < _wS.y)
                    return true;
            case pBottomRight:
                if (inMousePos.x >= (_wS.x - inMenuW) && inMousePos.x < _wS.x && inMousePos.y >= (_wS.y - inMenuH) &&
                    inMousePos.y < _wS.y)
                    return true;
            case pTopLeft:
                if (inMousePos.x >= 0 && inMousePos.x < inMenuW && inMousePos.y >= 0 && inMousePos.y < inMenuH)
                    return true;
            case pTopRight:
                if (inMousePos.x >= (_wS.x - inMenuW) && inMousePos.x < _wS.x && inMousePos.y >= 0 &&
                    inMousePos.y < inMenuH)
                    return true;
            case pCenter:
            case pCenterBottom:
            case pCenterTop:
                break;
        }
        return false;
    }

protected:
    std::shared_ptr<rIMenu> rIM_parentMenu;
    gTileset rIM_tsTex;
    sf::VertexArray v;
    std::vector<std::vector<sf::Vector2f>> rIM_lRefTiles;
    rRelativePos rIM_rPos;

    struct defTxtCompany {
        std::pair<uint32_t, uint32_t> pStartText;
        uint8_t pLength;
    };
    std::vector<defTxtCompany> rIM_comV;

    sf::VertexArray rIM_dInfo;
    int rIM_gWidth = 0;
    int rIM_gHeight = 0;
    std::vector<std::vector<int>> rIM_dExtracted;

    struct of_button {
        uint32_t ofb_pX;
        uint32_t ofb_pY;
        uint32_t ofb_pWidth;
        uint32_t ofb_pHeight;
    };
    std::vector<of_button> rIM_comVButtons;
    std::vector<of_button> rIM_comVEyes;
    std::vector<bool> rIM_comVEyesState;

    struct rIMenu_objFile {
        std::vector<std::vector<int>> of_data;
        uint32_t of_inSelTex;
        std::vector<uint8_t> of_inLengthTex;
        std::vector<of_button> of_vecButton;
        std::vector<of_button> of_vecEye;
    };

    static rIMenu_objFile extractDataFromFile(const std::string &pthFileD) {
        std::map<std::string, std::string> _strValRes = ReaderParameters::readFile(
                (RelPath::relPath / "files" / "graphic" / "menus" / (pthFileD + R"(.txt)")).string());

        std::ifstream _file((RelPath::relPath / "files" / "graphic" / "menus" / _strValRes["src"]).string());

        std::string _line;
        std::vector<std::vector<int>> _gContentF;
        while (std::getline(_file, _line)) {
            std::vector<int> row;
            std::istringstream iss(_line);
            int value;
            while (iss >> value) {
                row.push_back(value);
            }
            _gContentF.push_back(row);
        }
        _file.close();

        uint32_t _gSelTex = 0;
        if (_strValRes.find("inSelTex") != _strValRes.end())
            _gSelTex = std::stoul(_strValRes["inSelTex"]);

        std::vector<uint8_t> _gVecLengthTex = {};
        if (_strValRes.find("inLengthTex") != _strValRes.end()) {
            std::istringstream ss(_strValRes["inLengthTex"]);
            std::string segment;
            while (std::getline(ss, segment, ','))
                _gVecLengthTex.push_back(std::stoul(segment));
        }

        std::vector<of_button> _gVecPosButtons = {};
        if (_strValRes.find("inPosButtons") != _strValRes.end()) {
            std::istringstream ss(_strValRes["inPosButtons"]);
            std::string segment;
            while (std::getline(ss, segment, ',')) {
                std::istringstream ssInside(segment);
                std::string segmentInside;
                std::vector<uint32_t> _gVecComponent;
                while (std::getline(ssInside, segmentInside, '#'))
                    _gVecComponent.push_back(std::stoul(segmentInside));
                if (_gVecComponent.size() == 4)
                    _gVecPosButtons.push_back(
                            {_gVecComponent[0], _gVecComponent[1], _gVecComponent[2], _gVecComponent[3]});
            }
        }

        std::vector<of_button> _gVecPosEyes = {};
        if (_strValRes.find("inPosEyes") != _strValRes.end()) {
            std::istringstream ss(_strValRes["inPosEyes"]);
            std::string segment;
            while (std::getline(ss, segment, ',')) {
                std::istringstream ssInside(segment);
                std::string segmentInside;
                std::vector<uint32_t> _gVecComponent;
                while (std::getline(ssInside, segmentInside, '#'))
                    _gVecComponent.push_back(std::stoul(segmentInside));
                if (_gVecComponent.size() == 4)
                    _gVecPosEyes.push_back(
                            {_gVecComponent[0], _gVecComponent[1], _gVecComponent[2], _gVecComponent[3]});
            }
        }
        return {_gContentF, _gSelTex, _gVecLengthTex, _gVecPosButtons, _gVecPosEyes};
    }

    void setPositionValue(std::pair<int, int> cPos, uint32_t cValue) {
        sf::Vertex *quad = &rIM_dInfo[(cPos.second + cPos.first * rIM_gWidth) * 4];
        for (int k = 0; k < 4; k++)
            quad[k].texCoords = rIM_lRefTiles[cValue][k];
    }

    void setText(const uint8_t tVal, std::string cText) {

        if (rIM_rPos == rIMenu::rRelativePos::pBottomRight || rIM_rPos == rIMenu::rRelativePos::pTopRight) {
            if(rIM_comV[tVal].pLength > 6 && rIM_comV[tVal].pLength - cText.size() < 2)
                cText = cText.substr(0, 5) + "...";

            std::reverse(cText.begin(), cText.end());
            cText.append(rIM_comV[tVal].pLength - cText.size(), ' ');
        }

        for (int i = 0; i < rIM_comV[tVal].pLength; i++) {
            sf::Vertex *quad;

            if (rIM_rPos == rIMenu::rRelativePos::pBottomRight || rIM_rPos == rIMenu::rRelativePos::pTopRight)
                quad = &rIM_dInfo[(rIM_gWidth - 1 - rIM_comV[tVal].pStartText.second + i
                                   + rIM_comV[tVal].pStartText.first * rIM_gWidth) * 4];
            else
                quad = &rIM_dInfo[
                        (rIM_comV[tVal].pStartText.second + i + rIM_comV[tVal].pStartText.first * rIM_gWidth) * 4];

            char currentChar = (i < cText.size()) ? cText[i] : ' ';
            int charIndex;
            if (currentChar >= 'a' && currentChar <= 'z') charIndex = currentChar - 'a' + 1;
            else if (currentChar >= 'A' && currentChar <= 'Z') charIndex = currentChar - 'A' + 65;
            else if (currentChar >= '0' && currentChar <= '9') charIndex = currentChar - '0' + 48;
            else if (currentChar == '.') charIndex = 46;
            else if (currentChar == ':') charIndex = 58;
            else if (currentChar == '/') charIndex = 47;
            else if (currentChar == '-') charIndex = 45;
            else charIndex = 32;

            const int defaultCharIndex = 32;
            for (int k = 0; k < 4; k++) {
                quad[k].texCoords = rIM_lRefTiles[(charIndex != -1) ? charIndex : defaultCharIndex][k];
            }
        }
    }

    static std::string getFloatToString2Decimal(const float inValue) {
        std::stringstream ss;
        if (inValue >= 10000) {
            int exp = static_cast<int>(std::log10(inValue));
            if (exp % 3 == 0) {
                ss << std::fixed << static_cast<int>(inValue / std::pow(10, exp)) << "";
            } else if (exp % 3 == 1) {
                ss << std::fixed << static_cast<int>(inValue / std::pow(10, exp - 1)) << "k";
            } else {
                ss << std::fixed << static_cast<int>(inValue / std::pow(10, exp - 2)) << "M";
            }
        } else {
            ss << std::fixed << std::setprecision(2) << inValue;
        }
        return ss.str();
    }
};

#endif //CITYOFWEIRDFISHES_RIMENU_H
