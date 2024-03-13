//
// Created by Laminar on 13/03/2024.
//

#ifndef CITYOFWEIRDFISHES_RROADCROSSVIEW_H
#define CITYOFWEIRDFISHES_RROADCROSSVIEW_H

#include "rRoadViewMenu.h"

class rRoadCrossView : public rRoadViewMenu {
public:
    explicit rRoadCrossView(const std::shared_ptr<rIMenu> &mParent, const std::shared_ptr<rRNodeI> &refView,
                            rIMenu::rRelativePos rPos)
            : rRoadViewMenu(mParent, refView, "d_mRoadsViewCrossing", rPos) {
        std::vector<uint8_t> sLengths = {3, 3, 3, 3};
        comV = std::vector<defTxtCompany>(5, {{0, 0}});
        rCrossPos = std::vector<std::pair<int, int>>(5, {0, 0});
        int pAct = 0;
        int pCross = 0;
        for (int i = 0; i < dExtracted.size(); ++i) {
            for (int j = 0; j < dExtracted[i].size(); ++j) {
                auto row = (rPos == pBottomLeft || rPos == pBottomRight) ? dExtracted.size() - 1 - i : i;
                auto col = (rPos == pTopRight || rPos == pBottomRight) ? dExtracted[i].size() - 1 - j : j;
                if (dExtracted[i][j] == 65 || dExtracted[i][j] == 48) {
                    comV[pAct] = {{row, col}, sLengths[pAct]};
                    pAct++;
                } else if (dExtracted[i][j] == 277) {
                    rCrossPos[pCross] = {i, j};
                    pCross++;
                } else if(dExtracted[i][j] == 272){
                    rCenter = {i,j};
                }
            }
        }
        setSizeWays(rSelRoad->getSizesNeighbor());
    }

    void update() override {
        setRoadsCars(rSelRoad->getPosRoad(0));
    }

    void setSizeWays(const std::pair<std::pair<int, int>, std::pair<int, int>> &fOpening) {
        if (fOpening.first.first == -1)
            setText(0, " ");
        else {
            setText(0, std::to_string(fOpening.first.first));
            setPositionValue(rCrossPos[3], 32);
        }

        if (fOpening.first.second == -1)
            setText(3, " ");
        else {
            setText(3, std::to_string(fOpening.first.second));
            setPositionValue(rCrossPos[0], 32);
        }

        if (fOpening.second.first == -1)
            setText(1, " ");
        else {
            setText(1, std::to_string(fOpening.second.first));
            setPositionValue(rCrossPos[1], 32);
        }

        if (fOpening.second.second == -1)
            setText(2, " ");
        else {
            setText(2, std::to_string(fOpening.second.second));
            setPositionValue(rCrossPos[2], 32);
        }
    }

    void setPositionValue(std::pair<int, int> cPos, uint32_t cValue) {
        sf::Vertex *quad = &dInfo[(cPos.second + cPos.first * gWidth) * 4];
        for (int k = 0; k < 4; k++) {
            quad[k].texCoords = lRefTiles[cValue][k];
        }
    }

    void setText(const uint8_t tVal, const std::string &cText) {
        for (int i = 0; i < comV[tVal].pLength; i++) {
            sf::Vertex *quad = &dInfo[
                    (comV[tVal].pStartText.second + i + comV[tVal].pStartText.first * gWidth) * 4];
            for (int k = 0; k < 4; k++) {
                if (i >= cText.size())
                    quad[k].texCoords = lRefTiles[32][k];
                else if (cText[i] >= 'a' && cText[i] <= 'z')
                    quad[k].texCoords = lRefTiles[(cText[i] - 'a') + 1][k];
                else if (cText[i] >= 'A' && cText[i] <= 'Z')
                    quad[k].texCoords = lRefTiles[(cText[i] - 'A') + 65][k];
                else if (cText[i] >= '0' && cText[i] <= '9')
                    quad[k].texCoords = lRefTiles[(cText[i] - '0') + 48][k];
                else if (cText[i] == '.')
                    quad[k].texCoords = lRefTiles[46][k];
                else
                    quad[k].texCoords = lRefTiles[32][k];
            }
        }
    }

    void setRoadsCars(const std::list<uint32_t> &rEleDins) {
        setPositionValue(rCenter, rEleDins.empty()? 32 : 272);
    }

private:
    struct defTxtCompany {
        std::pair<uint32_t, uint32_t> pStartText;
        uint8_t pLength;
    };
    std::vector<std::pair<int, int>> rCrossPos;
    std::vector<defTxtCompany> comV;
    std::pair<int, int> rCenter;
};

#endif //CITYOFWEIRDFISHES_RROADCROSSVIEW_H
