//
// Created by Laminar on 01/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYCOMPILER_H
#define CITYOFWEIRDFISHES_SCOMPANYCOMPILER_H

#include <vector>
#include <cstdint>
#include "../../structure/obj/elements/objCompany.h"

class sCompanyCompiler {
public:
    struct sCCIntentions {
        enum sCCEnumIntentions {
            CELL_Buy,
            CELL_GiveRent,
            CELL_GainRent,
            CELL_Sell,
            OBJ_Produce
        };

        sCCEnumIntentions scc_type;
        uint32_t scc_addIdInfo;
        std::shared_ptr<objCompany> scc_objCompany;
    };

    static sCCIntentions intentionsFromLong(const uint64_t &cVal, const std::shared_ptr<objCompany> &objC) {
        switch (cVal >> 53 & 0xFF) { //TODO CREC QUE ESTA MALAMENT
            case 0:
                return {sCCIntentions::OBJ_Produce, static_cast<uint32_t>(cVal & 0xFFFFFFFF), objC};
            case 1:
                return {sCCIntentions::CELL_Buy, static_cast<uint32_t>((cVal & 0xFFFFFFFF % 4) + 1), objC};
            case 2:
                return {sCCIntentions::CELL_Sell, static_cast<uint32_t>(cVal & 0xFFFFFFFF), objC};
            case 3:
                return {sCCIntentions::CELL_GiveRent, static_cast<uint32_t>(cVal & 0xFFFFFFFF),
                        objC};
            case 4:
                return {sCCIntentions::CELL_GainRent, static_cast<uint32_t>((cVal & 0xFFFFFFFF % 4) + 1), objC};
            default:
                return {};
        }
    }

    static std::vector<sCCIntentions>
    givenCode(const std::vector<uint64_t> &cCode, const std::shared_ptr<objCompany> &objC,
              uint32_t cMaxInstructions = 256) {
        uint64_t cPC = 0;
        uint64_t cV1 = 0;
        uint64_t cV2 = 0;
        std::vector<sCCIntentions> cRet = {};

        uint32_t cIndInst = 0;
        while (cPC < cCode.size() && cIndInst < cMaxInstructions) {
            switch (cCode[cPC] >> 61) {
                case 0:
                    cPC = cCode[cPC] & 0x1FFFFFFFFFFFFFFF;
                    break;
                case 1:
                    if (cV1 != 0)
                        cPC = cCode[cPC] & 0x1FFFFFFFFFFFFFFF;
                    break;
                case 2:
                    cV1 = cV1 > cV2 ? 1 : 0;
                    break;
                case 3:
                    cV1 = cV1 < cV2 ? 1 : 0;
                    break;
                case 4:
                    cV2 = cV1;
                    switch ((uint8_t) (cCode[cPC] & 0x0F)) {
                        case 0:
                            cV1 = objC->c_uuid;
                            break;
                        case 1:
                            cV1 = objC->c_cOwn.size();
                            break;
                        case 2:
                            cV1 = objC->c_pOwn.size();
                            break;
                        case 3:
                            cV1 = objC->c_lUuidRentedSelf.size();
                            break;
                        case 4:
                            cV1 = objC->c_lUuidRentedOther.size();
                            break;
                        case 5:
                            if (objC->c_pOwn.find(static_cast<uint32_t>(cCode[cPC] & 0xFFFFFFFF)) != objC->c_pOwn.end())
                                cV1 = objC->c_pOwn.at(static_cast<uint32_t>(cCode[cPC] & 0xFFFFFFFF));
                            else
                                cV1 = 0;
                            break;
                    }
                    break;
                case 5:
                    cV2 = cV1;
                    cV1 = cCode[cPC] & 0x1FFFFFFFFFFFFFFF;
                    break;
                case 6:
                    cRet.push_back(intentionsFromLong(cCode[cPC], objC));
                    break;
                case 7:
                default:
                    break;
            }
            if ((cCode[cPC] >> 61) > 1)
                cPC++;
            cIndInst++;
        }
        return cRet;
    }
};

#endif //CITYOFWEIRDFISHES_SCOMPANYCOMPILER_H
