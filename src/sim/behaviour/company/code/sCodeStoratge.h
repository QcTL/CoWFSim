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

class sCodeStorage {
public:
    struct sCodeObj {
        std::vector<uint64_t> sCO_Code;
        uint32_t sCO_Score = 0;
    };

    sCodeStorage() {}

    void initNewCode(uint32_t uuidNewCompany) {
        sCS_Map[uuidNewCompany] = std::make_shared<sCodeObj>();
        sCS_Map[uuidNewCompany]->sCO_Code = getCommonSmartCode();
        sCS_Map[uuidNewCompany]->sCO_Score = 1000;
    };

    sCodeObj getCodeByUuid(uint32_t uuidCompany) {
        return *sCS_Map[uuidCompany];
    }

    void updateScoreCode(const uint32_t uuidCompany, const int cDiff) {
        if( sCS_Map[uuidCompany]->sCO_Score <= cDiff)
            sCS_Map[uuidCompany]->sCO_Score = 0;
        else
            sCS_Map[uuidCompany]->sCO_Score += cDiff;

        if (sCS_Map[uuidCompany]->sCO_Score <= sCS_Threshold &&
            sCS_Map[uuidCompany]->sCO_Score + cDiff >= sCS_Threshold) {
            cSC_aboveThreshold.push_back(sCS_Map[uuidCompany]);
        } else if (cDiff < 0 &&
                   sCS_Map[uuidCompany]->sCO_Score >= sCS_Threshold &&
                   sCS_Map[uuidCompany]->sCO_Score + cDiff <= sCS_Threshold) {
            cSC_aboveThreshold.remove(sCS_Map[uuidCompany]);
        }
    }

private:
    static std::shared_ptr<sCodeObj> getRandomElement(const std::list<std::shared_ptr<sCodeObj>> &sList) {
        std::srand(static_cast<unsigned int>(std::time(nullptr))); // TODO que sempre sigui el mateix
        std::size_t size = sList.size();
        std::size_t index = std::rand() % size;
        auto it = sList.begin();
        std::advance(it, index);
        return *it;
    }

    std::vector<uint64_t> getCommonSmartCode() {
        std::vector<uint64_t> rCode = getRandomElement(cSC_aboveThreshold)->sCO_Code;
        std::vector<uint64_t> rMutatedCode(rCode.begin(), rCode.end());
        //TODO Mutate
        return rMutatedCode;
    }

    std::list<std::shared_ptr<sCodeObj>> cSC_aboveThreshold;
    uint32_t sCS_Threshold{};
    std::map<uint32_t, std::shared_ptr<sCodeObj>> sCS_Map;
};

#endif //CITYOFWEIRDFISHES_SCODESTORATGE_H
