//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBTOGM_H
#define CITYOFWEIRDFISHES_TST_GBTOGM_H

#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/gBasicGrid.h"

int tst_gBtoGM() {

    std::shared_ptr<gIGrid<float>> ptr = std::make_shared<gBasicGrid<float>>(gBasicGrid<float>(30, 30,-1));
    std::vector<gBaseToGradientMinimum::gtmElement> vElem = {
            gBaseToGradientMinimum::gtmElement(0.0,0,0.0,0),
            gBaseToGradientMinimum::gtmElement(0.1,0,0.15,0),
            gBaseToGradientMinimum::gtmElement(0.15,0,0.03,0),};

    gBaseToGradientMinimum gBGM(vElem, {15,15}, ptr ,35); // Pass ptr by reference
    gBGM.generate();

    std::cout << *dynamic_cast<gBasicGrid<float> *>(ptr.get()) << std::endl;
    std::cout << "------------" << std::endl;
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GBTOGM_H
