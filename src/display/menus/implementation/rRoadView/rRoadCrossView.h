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

    void setRoadsCars(const std::vector<std::list<uint32_t>> &rEleDins) {
        setPositionValue(rCenter, rEleDins[0].empty()? 32 : 272);
    }

private:
    std::vector<std::pair<int, int>> rCrossPos;
    std::pair<int, int> rCenter;
};

#endif //CITYOFWEIRDFISHES_RROADCROSSVIEW_H
