


#include <filesystem>
#include "src/common/RelPath.h"
#include "src/tests/tst_gBasicWFCBasic.h"

int main() {
    RelPath::selRelPath(std::filesystem::current_path().parent_path());

    tst_gBasicWFCBasic();
    return 0;
}
