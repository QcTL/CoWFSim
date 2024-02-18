


#include <filesystem>
#include "src/common/RelPath.h"
#include "src/tests/tst_gBasicWFCBasic.h"
#include "SimInitialize.h"
#include "src/IO/ReaderParameters.h"
#include "src/tests/tst_gBasicPattern.h"

int main() {
    RelPath::selRelPath(std::filesystem::current_path().parent_path());

     std::map<std::string, std::string> sMVar = ReaderParameters::readFile(
                (RelPath::relPath / "files" / "import" / "impFile.txt").string());
        for (const auto &it: sMVar) {
            std::cout << it.first << " | " << it.second << " " << "\n";
        }
    std::cout << "------"<< std::endl;

    SimInitialize::givenMap(sMVar);
    //tst_gBasicPattern();

    return 0;
}
