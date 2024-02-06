


#include <filesystem>
#include "src/common/RelPath.h"
#include "src/tests/tst_gExtractRoadsCompressed.h"

int main() {

    RelPath::selRelPath(std::filesystem::current_path().parent_path());

    int n = 3;
    int nBlocks = 10;
    int x = 4;

    return ((x + nBlocks - n) % nBlocks)-1;

    //tst_gExtractRoadsCompressed();
    return 0;
}
