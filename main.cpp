


#include <filesystem>
#include "src/common/RelPath.h"
#include "src/IO/ReaderParameters.h"
#include "src/tests/tst_gERCComm.h"
#include "src/tests/tst_gVeh.h"
#include "SimInitialize.h"

int main() {
    RelPath::selRelPath(std::filesystem::current_path().parent_path());

    std::map<std::string, std::string> sMVar = ReaderParameters::readFile(
            (RelPath::relPath / "files" / "import" / "impFile.txt").string());
    for (const auto &it: sMVar) {
        std::cout << it.first << " | " << it.second << " " << "\n";
    }
    std::cout << "------" << std::endl;

    //SimInitialize::givenMap(sMVar);
    //tst_gBasicPattern();
    //tst_gMenus();
    SimInitialize::givenMap(sMVar);
    //SimInitializePreb::givenMap(sMVar);

    return 0;
}