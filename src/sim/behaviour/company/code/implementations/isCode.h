//
// Created by ganymede on 4/16/24.
//

#ifndef CITYOFWEIRDFISHES_ISCODE_H
#define CITYOFWEIRDFISHES_ISCODE_H

#include <cstdint>
#include <memory>
#include "../../../../structure/obj/sCommon.h"

class isCode {
public:
    [[nodiscard]] virtual std::shared_ptr<sCodeObj> getCodeByType(uint8_t inCompanyType) = 0;
};

#endif //CITYOFWEIRDFISHES_ISCODE_H
