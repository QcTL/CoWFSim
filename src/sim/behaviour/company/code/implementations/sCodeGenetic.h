//
// Created by ganymede on 4/16/24.
//

#ifndef CITYOFWEIRDFISHES_SCODEGENETIC_H
#define CITYOFWEIRDFISHES_SCODEGENETIC_H

#include <random>
#include <list>
#include "isCode.h"
#include "../../../../snCommonAtr.h"

class sCodeGenetic : public isCode {
public:
    sCodeGenetic() {
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

    void updateScoreCode(const std::shared_ptr<sCodeObj> &inCodeObj, const int cDiff) {
        if (inCodeObj->sCO_Score <= sCS_Threshold && inCodeObj->sCO_Score + cDiff >= sCS_Threshold) {
            cSC_aboveThreshold.push_back(inCodeObj);
        } else if (cDiff < 0 && inCodeObj->sCO_Score >= sCS_Threshold &&
                   inCodeObj->sCO_Score + cDiff <= sCS_Threshold) {
            cSC_aboveThreshold.remove(inCodeObj);
        }
    }

    [[nodiscard]] std::shared_ptr<sCodeObj> getCodeByType(uint8_t inCompanyType) override{
        std::shared_ptr<sCodeObj> rCode = getRandomElement(cSC_aboveThreshold);
        std::vector<uint64_t> rMutatedCode = pActionsMutate(5, rCode->sCO_Code);
        rCode->sCO_Code = rMutatedCode;
        return rCode;
    }

private:

    std::shared_ptr<sCodeObj> getRandomElement(const std::list<std::shared_ptr<sCodeObj>> &sList) {
        std::uniform_int_distribution<std::size_t> distribution(0, sList.size() - 1);
        return *std::next(sList.begin(), distribution(sCS_genRVal));
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
                case 3: {
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


    uint32_t sCS_Threshold{};
    std::mt19937 sCS_genRVal;
    std::list<std::shared_ptr<sCodeObj>> cSC_aboveThreshold;
};

#endif //CITYOFWEIRDFISHES_SCODEGENETIC_H
