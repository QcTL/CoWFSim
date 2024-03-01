


#include <filesystem>
#include <iostream>
#include "src/IO/ReaderParameters.h"
#include "src/common/RelPath.h"
#include "SimInitialize.h"
#include "src/tests/tst_gExtractRoadsCompressed.h"
#include "src/tests/tst_gMenus.h"
#include "src/tests/tst_gBasicNewWay.h"
#include "src/tests/tst_gVeh.h"

int main() {
    RelPath::selRelPath(std::filesystem::current_path().parent_path());

    std::map<std::string, std::string> sMVar = ReaderParameters::readFile(
            (RelPath::relPath / "files" / "import" / "impFile.txt").string());
    for (const auto &it: sMVar) {
        std::cout << it.first << " | " << it.second << " " << "\n";
    }
    std::cout << "------" << std::endl;

    tst_gVeh();
    //tst_gMenus();
    //tst_gBasicNewWay(sMVar);
    //SimInitialize::givenMap(sMVar);

    return 0;
}