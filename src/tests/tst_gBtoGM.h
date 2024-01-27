//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBTOGM_H
#define CITYOFWEIRDFISHES_TST_GBTOGM_H

#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/gBasicGrid.h"

int tst_gBtoGM() {

    std::vector<gBaseToGradientMinimum::gtmElement> vElem = {
            gBaseToGradientMinimum::gtmElement(0.0,0,0.0,0),
            gBaseToGradientMinimum::gtmElement(0.1,0,0.15,0),
            gBaseToGradientMinimum::gtmElement(0.15,0,0.03,0),};

    gBaseToGradientMinimum gBGM(vElem, {15,15}, std::make_unique<gBasicGrid<float>>(gBasicGrid<float>(300, 30,-1)),35);
    gBGM.generate();
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GBTOGM_H
