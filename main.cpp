#include <filesystem>
#include <iostream>
#include "src/IO/ReaderParameters.h"
#include "src/common/RelPath.h"
#include "SimInitialize.h"
#include "src/common/r2BitDirection.h"
#include "src/tests/tst_gVeh.h"
#include "src/sim/snCommonAtr.h"

int main() {
    RelPath::selRelPath(std::filesystem::current_path().parent_path());
    r2BitDirection::loadPossibleRoads("FRoadValid.txt");

    std::map<std::string, std::string> sMVar = ReaderParameters::readFile(
            (RelPath::relPath / "files" / "import" / "impFile.txt").string());
    for (const auto &it: sMVar) {
        std::cout << it.first << " | " << it.second << " " << "\n";
    }
    std::cout << "------" << std::endl;
    snCommonAtr::setFlagAtr("snCA_Seed", std::stoi(sMVar["Llavor"]));

    SimInitialize::givenMap(sMVar);
    return 0;
}