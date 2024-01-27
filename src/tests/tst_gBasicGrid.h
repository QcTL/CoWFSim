//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_GBASICGRID_H
#define CITYOFWEIRDFISHES_TST_GBASICGRID_H

#include "../sim/structure/grids/gBasicGrid.h"

int tst_gBasicGrid() {
    gBasicGrid<int> gB(4, 4);
    std::cout << gB << std::endl;

    gB.set(0, 0, 5);
    gB.set(2, 3, 5);

    std::cout << gB << std::endl;

    std::cout << "(2,3): " << gB.get(2, 3) << std::endl;
    std::cout << "RangeX: " << gB.rangeUse().first.first << "-" << gB.rangeUse().first.second  << std::endl;
    std::cout << "RangeY: " << gB.rangeUse().second.first << "-" << gB.rangeUse().second.second  << std::endl;
    std::cout << std::endl;


    gBasicGrid<int> gB2(2, 4);
    gB2.set(1, 1, 1);
    std::cout << gB2 << std::endl;
    std::cout << "RangeX: " << gB2.rangeUse().first.first << "-" << gB2.rangeUse().first.second  << std::endl;
    std::cout << "RangeY: " << gB2.rangeUse().second.first << "-" << gB2.rangeUse().second.second  << std::endl;

    std::cout << std::endl;
    gB2.set(1, 10, 1);
    std::cout << gB2 << std::endl;
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_GBASICGRID_H
