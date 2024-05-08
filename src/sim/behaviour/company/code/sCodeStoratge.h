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
#include "../../interface/iStorage.h"

class sCodeStorage : public iStorage<sCodeObj, sCodeBase> {
public:
    sCodeStorage() {
        sCS_typeCodeGen = std::make_shared<sCodeManual>();
        //sCS_typeCodeGen = std::make_shared<sCodeGenetic>();

        sCS_nActNum = 0;
    }

    uint32_t addElement(const sCodeBase &inAddElem) override {
        sCS_Map[inAddElem.uuidNewCompany] =
                sCS_typeCodeGen->getCodeByType(inAddElem.typeStartCompany);
        return sCS_nActNum++;
    }

    std::shared_ptr<sCodeObj> getElementByUuid(uint32_t inUuidElem) override {
        return sCS_Map[inUuidElem];
    }

    bool removeElementByUuid(uint32_t inUuidElem) override{
        sCS_Map[inUuidElem] = nullptr;
        return true;
    }

    void updateScoreCode(const uint32_t uuidCompany, const int cDiff) {
        if (sCS_Map[uuidCompany]->sCO_Score <= cDiff)
            sCS_Map[uuidCompany]->sCO_Score = 0;
        else
            sCS_Map[uuidCompany]->sCO_Score += cDiff;
    }

private:
    uint32_t sCS_nActNum;
    std::shared_ptr<isCode> sCS_typeCodeGen;
    std::map<uint32_t, std::shared_ptr<sCodeObj>> sCS_Map;
};

#endif //CITYOFWEIRDFISHES_SCODESTORATGE_H
