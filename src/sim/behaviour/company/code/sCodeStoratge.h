//
// Created by ganymede on 3/19/24.
//

#ifndef CITYOFWEIRDFISHES_SCODESTORATGE_H
#define CITYOFWEIRDFISHES_SCODESTORATGE_H

#include <cstdint>
#include <vector>
#include <map>
#include <list>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "../../../snCommonAtr.h"
#include "implementations/isCode.h"
#include "implementations/sCodeManual.h"
#include "implementations/sCodeGenetic.h"

class sCodeStorage {
public:
    sCodeStorage() {
        sCS_typeCodeGen = std::make_shared<sCodeManual>();
        //sCS_typeCodeGen = std::make_shared<sCodeGenetic>();
    }

    void initNewCode(uint32_t uuidNewCompany, uint8_t typeStartCompany) {
        sCS_Map[uuidNewCompany] = sCS_typeCodeGen->getCodeByType(typeStartCompany);
    };

    sCodeObj getCodeByUuid(uint32_t uuidCompany) {
        return *sCS_Map[uuidCompany];
    }

    std::shared_ptr<sCodeObj> getPointerCodeByUuid(uint32_t uuidCompany) {
        return sCS_Map[uuidCompany];
    }

    void updateScoreCode(const uint32_t uuidCompany, const int cDiff) {
        if (sCS_Map[uuidCompany]->sCO_Score <= cDiff)
            sCS_Map[uuidCompany]->sCO_Score = 0;
        else
            sCS_Map[uuidCompany]->sCO_Score += cDiff;
    }

private:
    std::shared_ptr<isCode> sCS_typeCodeGen;
    std::map<uint32_t, std::shared_ptr<sCodeObj>> sCS_Map;
};

#endif //CITYOFWEIRDFISHES_SCODESTORATGE_H
