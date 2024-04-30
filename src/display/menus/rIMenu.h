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

/**
 * @class rIMenu
 * @brief Interface class that has to be implemented to work as a menu
 *
 * Interface class with the necessary functions to perform all the requirements of a menu.
 */
class rIMenu {
public:

    /**
     * @enum rRelativePos
     * @brief Enum that defines all the custom positions that the menu can have
     */
    enum rRelativePos {
        pBottomLeft, pBottomRight, pTopLeft, pTopRight, pCenter, pCenterBottom, pCenterTop
    };

    /**
     * @brief Constructor of the menu with the required parameters to set the common attributes or actions of the menus
     * @param mParent Reference to the menu that has called for the creation of this one, nullptr if first
     * @param rRelativePos Relative position that will occupy the menu when draw
     * @param pthFileD Path to the file descriptor that sets the contents of the menu
     */
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
            rIM_idMenu = _dInfoFile.of_nameFile;
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

    /**
     * @brief Function to change the graphical appearance of the eye sprite in the menu to reflect its state
     * @param inUuidEye Id of the eye tracked, has to be between 0 and the max number of stipulated eyes in the file descriptor for the menu
     * @param inNewStateValue New state that the sprite of the eye will have
     */
    virtual void setEyeVisualValue(uint32_t inUuidEye, bool inNewStateValue) {
        sf::Vertex *_quad;
        switch (rIM_rPos) {
            case pBottomLeft:
                _quad = &rIM_dInfo[(rIM_comVEyes[inUuidEye].ofb_pX +
                                    (rIM_gHeight - 1 - rIM_comVEyes[inUuidEye].ofb_pY) * rIM_gWidth) * 4];
                break;
            case pBottomRight:
                _quad = &rIM_dInfo[(rIM_gWidth - 1 - rIM_comVEyes[inUuidEye].ofb_pX +
                                    (rIM_gHeight - 1 - rIM_comVEyes[inUuidEye].ofb_pY) * rIM_gWidth) * 4];
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

    /**
     * @fn void draw
     * @brief Function to implement that defines how the window is set to draw on screen
     * @param inRenderWin The renderWindow where the screen will be displayed.
     */
    virtual void draw(sf::RenderWindow &inRenderWin) {
        inRenderWin.draw(rIM_dInfo, &rIM_tsTex.tsTex);
    };

    /**
     * @fn void setResponse
     * @brief Function that will be called from the menu created from this menu as a form of communication
     * With this function you can set the result of the menu opened after this one, or send the event necessary for its closure
     * @param inValResponse Value returned from the child menu
     * @param inLIDSender Uuid String of the child menu defined on the specifications .txt of the menu.
     */
    virtual void setResponse(int inValResponse, const std::string &inLIDSender) {};

    /**
     * @fn bool interact
     * @brief Function that is called when an event occurs with this menu active on the screen.
     * @param inEvent The event that happened
     * @param inRenderWin The window in which that event happened
     * @return True if the input has been dealt and it dost have to transition into the father menu, false otherwise
     */
    virtual bool interact(const sf::Event &inEvent, const sf::RenderWindow &inRenderWin) = 0;

    /**
     * @fn void pressedCell
     * @brief Function that is called when a cell of the grid is pressed with this menu as the top one. So without active children
     * @param cPressed Coordinates of the cell pressed
     * @param inPTime Reduced time when it was pressed
     * @param inCDate Reduced date when it was pressed
     */
    virtual void pressedCell(std::pair<int, int> cPressed, uint32_t inPTime, uint32_t inCDate){};

    /**
     * @fn void update
     * @brief Function that is called every frame to change if its necessary the menu in a dynamic way.
     */
    virtual void update() {}

    /**
     * @fn  rRelativePos getRPos
     * @return Get the relative position of the menu, one possible value of @ref rRelativePos
     */
    rRelativePos getRPos() {
        return rIM_rPos;
    }

    /**
     * @fn sf::Vector2<int> getRelPosMenu
     * @brief Function used to get the relative position of the sprite pressed on a given menu.
     * @param inRenderWin The actual render window where the menu is displayed
     * @param inMousePos The real position of the mouse on the screen
     * @return A Vector of 2 positions with the relative coordinates of the pressed sprite on the menu. Where 0,0 its top left.
     */
    sf::Vector2<int> getRelPosMenu(const sf::RenderWindow &inRenderWin, const sf::Vector2<int> &inMousePos) {
        sf::Vector2<int> _mouseRel = {inMousePos.x / 16, inMousePos.y / 16};
        sf::Vector2<int> _winSize = {(int) inRenderWin.getSize().x / 16, (int) inRenderWin.getSize().y / 16};

        if (rIM_rPos == rRelativePos::pBottomRight || rIM_rPos == rRelativePos::pTopRight)
            _mouseRel.x = _mouseRel.x - (_winSize.x - rIM_gWidth);

        if (rIM_rPos == rRelativePos::pBottomRight || rIM_rPos == rRelativePos::pBottomLeft)
            _mouseRel.y = _mouseRel.y - (_winSize.y - rIM_gHeight) - 1;

        return _mouseRel;
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

    /**
     * @fn int getButtonPressed
     * @brief Given the mouse position and the window in which the menu is rendered give the index of the button you pressed
     * @param inRenderWin The actual render window where the menu is displayed
     * @param inMousePos The real position of the mouse on the screen
     * @return If you pressed a button will return the index, otherwise, you will get -1
     */
    int getButtonPressed(const sf::RenderWindow &inRenderWin, const sf::Vector2<int> &inMousePos) {
        sf::Vector2<int> _mouseRel = getRelPosMenu(inRenderWin, inMousePos);
        for (int i = 0; i < rIM_comVButtons.size(); i++)
            if (_mouseRel.x >= rIM_comVButtons[i].ofb_pX && _mouseRel.y >= rIM_comVButtons[i].ofb_pY &&
                _mouseRel.x <= (rIM_comVButtons[i].ofb_pX + rIM_comVButtons[i].ofb_pWidth) &&
                _mouseRel.y <= (rIM_comVButtons[i].ofb_pY + rIM_comVButtons[i].ofb_pHeight))
                return i;
        return -1;
    }

    /**
        * @fn int getEyePressed
        * @brief Given the mouse position and the window in which the menu is rendered give the index of the eye you pressed
        * @param inRenderWin The actual render window where the menu is displayed
        * @param inMousePos The real position of the mouse on the screen
        * @return If you pressed a eye will return the index, otherwise, you will get -1
        */
    int getEyePressed(const sf::RenderWindow &inRenderWin, const sf::Vector2<int> &inMousePos) {
        sf::Vector2<int> _mouseRel = getRelPosMenu(inRenderWin, inMousePos);
        for (int i = 0; i < rIM_comVEyes.size(); i++)
            if (_mouseRel.x >= rIM_comVEyes[i].ofb_pX && _mouseRel.y >= rIM_comVEyes[i].ofb_pY &&
                _mouseRel.x <= (rIM_comVEyes[i].ofb_pX + rIM_comVEyes[i].ofb_pWidth) &&
                _mouseRel.y <= (rIM_comVEyes[i].ofb_pY + rIM_comVEyes[i].ofb_pHeight))
                return i;
        return -1;
    }

    /**
     * @fn bool isInside
     * @param inRenderWin  The actual render window where the menu is displayed
     * @param inMenuH  The height of tiles of the menu
     * @param inMenuW The weight of tiles of the menu
     * @param inMousePos The real position of the mouse on the screen
     * @return
     */
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
    std::string rIM_idMenu;

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
        std::string of_nameFile;
        uint32_t of_inSelTex;
        std::vector<uint8_t> of_inLengthTex;
        std::vector<of_button> of_vecButton;
        std::vector<of_button> of_vecEye;
    };

    /**
     * @fn  rIMenu_objFile extractDataFromFile
     * @param pthFileD given the relative path of the descriptor .txt of the menu
     * @return A struct having all the attributes of the file.
     */
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

        std::string _sNameMenu = _strValRes["type"];

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
        return {_gContentF, _sNameMenu, _gSelTex, _gVecLengthTex, _gVecPosButtons, _gVecPosEyes};
    }

    /**
     * @fn void setPositionValue
     * @param cPos The position where you want to change the sprite in the menu
     * @param cValue The new value you wan the sprite to have
     */
    void setPositionValue(std::pair<int, int> cPos, uint32_t cValue) {
        sf::Vertex *quad = &rIM_dInfo[(cPos.second + cPos.first * rIM_gWidth) * 4];
        for (int k = 0; k < 4; k++)
            quad[k].texCoords = rIM_lRefTiles[cValue][k];
    }

    /**
     * @fn void setText
     * @param tVal The index of the position where it can write, 0 < x < rIM_comV.size()
     * @param cText The text you want to display starting from that position
     */
    void setText(const uint8_t tVal, std::string cText) {

        if (rIM_rPos == rIMenu::rRelativePos::pBottomRight || rIM_rPos == rIMenu::rRelativePos::pTopRight) {
            if (rIM_comV[tVal].pLength > 6 && (int) rIM_comV[tVal].pLength - (int) cText.size() < 2)
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

    /**
     * @fn std::string getFloatToString2Decimal
     * @brief given a float return the common numerical way of typing that number, with "k" and "M"
     * @param inValue The value you want represented
     * @return A string containing the text representation of the number
     */
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

private:

    /** @fn void setTransformations
     * @brief Given the tiles of the file, it correctly permutes it so it looks correct in any relative position it can be presented.
     */
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

    /**
     * @fn sf::VertexArray getVertexMenu
     * @brief Given the sprite information of the menu, returns the vertex position it has to have to correctly represent it.
     * @param GWidth The Width of the menu in tiles
     * @param GHeight The Height of the menu in tiles
     * @param strData The matrix of the sprite values of each position
     * @return The Vertex array that describes the positions of the coord in the tileset.
     */
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

};

#endif //CITYOFWEIRDFISHES_RIMENU_H
