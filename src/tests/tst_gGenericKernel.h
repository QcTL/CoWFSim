//
// Created by Laminar on 28/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GKERNEL_H
#define CITYOFWEIRDFISHES_TST_GKERNEL_H

#include "../sim/structure/grids/transformation/gBaseToGradientMinimum.h"
#include "../sim/structure/grids/gBasicGrid.h"
#include "../sim/structure/grids/transformation/gBasicTransformations.h"

int tst_gGenericKernel() {

    std::shared_ptr<gIGrid<double>> ptr = std::make_shared<gBasicGrid<double>>(gBasicGrid<double>(100, 100, -1));


    BasicTransformations::Kernel<double> k({0.0625,0.125, 0.0625,0.125,0.25,0.125,0.0625,0.125, 0.0625},3,3);

    BasicTransformations::GenericKernelTransformation<double>(ptr, k);

    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GKERNEL_H
