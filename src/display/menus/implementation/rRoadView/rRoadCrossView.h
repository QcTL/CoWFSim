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
                int val = dExtracted[i][j];
                if (val == 65 || val == 48) comV[pAct++] = {{row, col}, sLengths[pAct]};
                else if (val == 277) rCrossPos[pCross++] = {i, j};
                else if (val == 272) rCenter = {i, j};
            }
        }
        setSizeWays(rSelRoad->getSizesNeighbor());
    }

    void update() override {
        setRoadsCars(rSelRoad->getPosRoad(0));
    }

    std::vector<uint8_t> rOrderDir = {0, 3, 1, 2};
    std::vector<uint8_t> rOrderCross = {3, 0, 1, 2};

    void setSizeWays(const std::vector<int> &fOpening) {
        for(int i =0; i < 4; i++){
            setText(rOrderDir[i], fOpening[i] == -1 ? " " : std::to_string(fOpening[i]));
            if (fOpening[i] != -1)
                setPositionValue(rCrossPos[rOrderCross[i]], 32);
        }
    }

    void setRoadsCars(const std::vector<std::list<uint32_t>> &rEleDins) {
        setPositionValue(rCenter, rEleDins[0].empty() ? 32 : 272);
    }

private:
    std::vector<std::pair<int, int>> rCrossPos;
    std::pair<int, int> rCenter;
};

#endif //CITYOFWEIRDFISHES_RROADCROSSVIEW_H
