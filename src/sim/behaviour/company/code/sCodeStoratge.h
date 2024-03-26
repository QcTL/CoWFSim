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

        sCodeObj(const std::vector<uint64_t> &sCoCode, uint32_t sCoScore) : sCO_Code(sCoCode), sCO_Score(sCoScore) {}
    };

    sCodeStorage() {
        cSC_aboveThreshold.push_back(std::make_shared<sCodeObj>(sCodeObj(
                {
                        (uint64_t) 0b1010000000000000000000000000000000000000000000000000000011001000,
                        (uint64_t) 0b1000000000000000000000000000000000000000000000000000000000000100,
                        (uint64_t) 0b0110000000000000000000000000000000000000000000000000000000000000,
                        (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                        (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000010,
                        (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,},
                1000)));
    }

    void initNewCode(uint32_t uuidNewCompany) {
        sCS_Map[uuidNewCompany] = getCommonSmartCode();
        sCS_Map[uuidNewCompany]->sCO_Score = 1000;
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

    std::shared_ptr<sCodeObj> getCommonSmartCode() {
        std::shared_ptr<sCodeObj> rCode = getRandomElement(cSC_aboveThreshold);
        std::vector<uint64_t> rMutatedCode(rCode->sCO_Code.begin(), rCode->sCO_Code.end());
        //TODO Mutate
        return rCode;
    }

    std::list<std::shared_ptr<sCodeObj>> cSC_aboveThreshold;
    uint32_t sCS_Threshold{};
    std::map<uint32_t, std::shared_ptr<sCodeObj>> sCS_Map;
};

#endif //CITYOFWEIRDFISHES_SCODESTORATGE_H
