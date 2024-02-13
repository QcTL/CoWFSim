


#include <filesystem>
#include "src/common/RelPath.h"
#include "src/tests/tst_gERCComm.h"

int main() {
    RelPath::selRelPath(std::filesystem::current_path().parent_path());

    tst_gERCComm();
    return 0;
}
