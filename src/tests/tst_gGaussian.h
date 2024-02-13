//
// Created by Laminar on 28/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GGAUSSIAN_H
#define CITYOFWEIRDFISHES_TST_GGAUSSIAN_H

#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"

int tst_gGaussian() {

    std::shared_ptr<gIGrid<double>> ptr = std::make_shared<gBasicGrid<double>>(gBasicGrid<double>(10, 8, -1));
    std::vector<gtmElement> vElem = {
            gtmElement(0.0, 0, 0.0, 0),
            gtmElement(0.05, 0, 0.1, 0),
            gtmElement(0.25, 0, 0.1, 0),};

    gBaseToGradientMinimum<double> gBGM(vElem, {5, 5}, ptr, 35); // Pass ptr by reference
    gBGM.generateV1();

    std::cout << *dynamic_cast<gBasicGrid<double> *>(ptr.get()) << std::endl;
    std::cout << "------------" << std::endl;

    BasicTransformations::GaussianFilter<double>(ptr);

    std::cout << *dynamic_cast<gBasicGrid<double> *>(ptr.get()) << std::endl;
    std::cout << "------------" << std::endl;
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GGAUSSIAN_H
