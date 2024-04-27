//
// Created by ganymede on 4/27/24.
//

#ifndef CITYOFWEIRDFISHES_RROADPRESENTVIEW_H
#define CITYOFWEIRDFISHES_RROADPRESENTVIEW_H

#include "../../rIMenu.h"
#include "../../../../sim/roads/node/rRNode.h"

class rRoadPresentView : public rIScrollableMenu<rRNodeL::rPV_Car_Export> {
public:
    rRoadPresentView(const std::shared_ptr<rIMenu> &inMParent, const std::shared_ptr<rRNodeI> &refView)
            : rIScrollableMenu<rRNodeL::rPV_Car_Export>(inMParent, rIMenu::rRelativePos::pTopLeft,
                                                        "d_mRoadsViewPresentLayer", 4),
              rEPV_refRoad(refView) {
        rPVL_totalElements = rEPV_refRoad->exportCarsPresent();
        printItemsOnScreen();
    }

protected:

    void printItemsOnScreen() override {
        for (int i = 0; i < rPVL_nObjInLayer * 3; i += 3) {
            uint32_t vObjAct = rPVL_cCurLayerObjects * rPVL_nObjInLayer + i / 3;
            if (vObjAct < rPVL_totalElements.size()) {
                setText(i, std::to_string(rPVL_totalElements[vObjAct].rPV_uuidCar));
                setText(i + 1, std::to_string(rPVL_totalElements[vObjAct].rPV_destination.first));
                setText(i + 2, std::to_string(rPVL_totalElements[vObjAct].rPV_destination.second));
            } else
                for (int j = 0; j < 3; ++j)
                    setText(i + j, "");

        }

    }

private:
    std::shared_ptr<rRNodeI> rEPV_refRoad;
};

#endif //CITYOFWEIRDFISHES_RROADPRESENTVIEW_H
