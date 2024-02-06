


#include <filesystem>
#include "src/common/RelPath.h"
#include "src/tests/tst_gMenus.h"

int main() {
    RelPath::selRelPath(std::filesystem::current_path().parent_path());

    tst_gMenus();
    return 0;
}
