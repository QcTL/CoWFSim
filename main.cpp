


#include <filesystem>
#include <iostream>
#include "src/IO/ReaderParameters.h"
#include "src/common/RelPath.h"
#include "SimInitialize.h"

int main() {
    RelPath::selRelPath(std::filesystem::current_path().parent_path());

    std::map<std::string, std::string> sMVar = ReaderParameters::readFile(
            (RelPath::relPath / "files" / "import" / "impFile.txt").string());
    for (const auto &it: sMVar) {
        std::cout << it.first << " | " << it.second << " " << "\n";
    }
    std::cout << "------" << std::endl;

    SimInitialize::givenMap(sMVar);

    return 0;
}