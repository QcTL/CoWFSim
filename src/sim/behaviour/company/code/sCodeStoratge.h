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

        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            sCS_genRVal.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            sCS_genRVal.seed(static_cast<unsigned int>(time(nullptr)));
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
    std::shared_ptr<sCodeObj> getRandomElement(const std::list<std::shared_ptr<sCodeObj>> &sList) {
        std::uniform_int_distribution<std::size_t> distribution(0, sList.size() - 1);
        return *std::next(sList.begin(), distribution(sCS_genRVal));
    }

    std::shared_ptr<sCodeObj> getCommonSmartCode() {
        std::shared_ptr<sCodeObj> rCode = getRandomElement(cSC_aboveThreshold);
        std::vector<uint64_t> rMutatedCode = pActionsMutate(5, rCode->sCO_Code);
        rCode->sCO_Code = rMutatedCode;
        return rCode;
    }

    std::vector<uint64_t> pActionsMutate(const uint32_t inNMutations, const std::vector<uint64_t> &inSCode) {
        std::list<uint64_t> sCodeLst(inSCode.begin(), inSCode.end());
        std::uniform_int_distribution<uint8_t> distRMutation(0, 10);

        for (int i = 0; i < inNMutations; i++) {
            std::uniform_int_distribution<std::size_t> dPosAdd(0, sCodeLst.size() - 1);
            switch (distRMutation(sCS_genRVal)) {
                case 0:  //Add a new line
                {
                    auto it = sCodeLst.begin();
                    std::advance(it, dPosAdd(sCS_genRVal));
                    sCodeLst.insert(it, (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000);
                }
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6: {
                    auto it = sCodeLst.begin();
                    std::advance(it, dPosAdd(sCS_genRVal));

                    std::uniform_int_distribution<uint64_t> bitPos(0, sizeof(uint64_t) * 8 - 1);
                    uint64_t bitIndex = bitPos(sCS_genRVal);
                    *it ^= (1ULL << bitIndex);
                }
                    break;
                default:
                    break;
            }
        }
        return std::vector<uint64_t>(sCodeLst.begin(), sCodeLst.end());
    }

    std::list<std::shared_ptr<sCodeObj>> cSC_aboveThreshold;
    uint32_t sCS_Threshold{};
    std::map<uint32_t, std::shared_ptr<sCodeObj>> sCS_Map;

    std::mt19937 sCS_genRVal;
};

#endif //CITYOFWEIRDFISHES_SCODESTORATGE_H
