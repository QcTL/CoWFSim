//
// Created by Laminar on 28/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GGAUSSIAN_H
#define CITYOFWEIRDFISHES_TST_GGAUSSIAN_H

#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"

int tst_gGaussian() {

    std::shared_ptr<gIGrid<float>> ptr = std::make_shared<gBasicGrid<float>>(gBasicGrid<float>(10, 8,-1));
    std::vector<gBaseToGradientMinimum::gtmElement> vElem = {
            gBaseToGradientMinimum::gtmElement(0.0,0,0.0,0),
            gBaseToGradientMinimum::gtmElement(0.05,0,0.1,0),
            gBaseToGradientMinimum::gtmElement(0.25,0,0.1,0),};

    gBaseToGradientMinimum gBGM(vElem, {5,5}, ptr ,35); // Pass ptr by reference
    gBGM.generate();

    std::cout << *dynamic_cast<gBasicGrid<float> *>(ptr.get()) << std::endl;
    std::cout << "------------" << std::endl;

    BasicTransformations::GaussianFilter<float>(ptr);

    std::cout << *dynamic_cast<gBasicGrid<float> *>(ptr.get()) << std::endl;
    std::cout << "------------" << std::endl;
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GGAUSSIAN_H
