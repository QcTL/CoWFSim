//
// Created by Laminar on 27/01/2024.
//

#ifndef CITYOFWEIRDFISHES_TST_READER_H
#define CITYOFWEIRDFISHES_TST_READER_H
//
// Created by Laminar on 27/01/2024.
//

#include <iostream>
#include "../IO/ReaderParameters.h"

int tst_ReaderV1()
{

    for(int i = 1; i < 3; i++) {
        std::map<std::string, std::string> sm = ReaderParameters::readFile(
                R"(C:\Users\Robert\CityOfWeirdFishes\files\test\rReaderTest)"+ std::to_string(i) +R"(.txt)");

        for (const auto &it: sm) {
            std::cout << it.first << " | " << it.second << " " << "\n";
        }
        std::cout << "------"<< std::endl;
    }
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_READER_H
