


#include <filesystem>
#include "src/tests/tst_gExtractRoads.h"
#include "src/common/RelPath.h"
#include "src/tests/tst_Reader.h"

int main() {

    RelPath::selRelPath(std::filesystem::current_path().parent_path());
    //tst_gExtractRoads();
    tst_ReaderV1();
    return 0;
}
