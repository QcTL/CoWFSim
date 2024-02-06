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
#include "../common/RelPath.h"

int tst_ReaderV1()
{
    std::cout << (RelPath::relPath / "files" / "test" / ("rReaderTest" + std::to_string(0) + R"(.txt)")).string() << std::endl;
    for(int i = 1; i < 3; i++) {
        std::map<std::string, std::string> sm = ReaderParameters::readFile(
                (RelPath::relPath / "files" / "test" / ("rReaderTest" + std::to_string(i) + R"(.txt)")).string());
        for (const auto &it: sm) {
            std::cout << it.first << " | " << it.second << " " << "\n";
        }
        std::cout << "------"<< std::endl;
    }
    return 0;
}

#endif //CITYOFWEIRDFISHES_TST_READER_H
