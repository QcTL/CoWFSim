//
// Created by Laminar on 01/03/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMPANYCOMPILER_H
#define CITYOFWEIRDFISHES_SCOMPANYCOMPILER_H

#include <vector>
#include <cstdint>
#include "../structure/obj/sCommon.h"

class sCompanyCompiler {

    enum sCCIntentions {
    };

    static sCCIntentions intentionsFromLong(const uint64_t &cVal) {
        return sCCIntentions();
    }

    static std::vector<sCCIntentions>
    givenCode(const std::vector<uint64_t> &cCode, const objCompany &objC, uint32_t cMaxInstructions = 256) {
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
                            cV1 = objC.c_uuid;
                            break;
                        case 1:
                            cV1 = objC.c_cOwn.size();
                            break;
                        case 2:
                            cV1 = objC.c_pOwn.size();
                            break;
                        case 3:
                            cV1 = objC.c_cRentedOther.size();
                            break;
                        case 4:
                            cV1 = objC.c_cRentedSelf.size();
                            break;
                        case 5:
                            ///TODO EN ALGUN LLOC HA DE PODER ACCEDIR A LA SEVA QUANTIAT DE RESERVA DE OBJECTES CONCRETS, POTSER FER-HO AL REVES,
                            // MIRAR ELS BITS MES ALTS, I SI AQUEST ES UN CONCRET ES MIRA ELS 32 MES BAIXOS I ES PILLARA AQUELL
                            break;
                    }
                    break;
                case 5:
                    cV2 = cV1;
                    cV1 = cCode[cPC] & 0x1FFFFFFFFFFFFFFF;
                    break;
                case 6:
                    cRet.push_back(intentionsFromLong(cCode[cPC] & 0x1FFFFFFFFFFFFFFF));
                    break;
                case 7:
                default:
                    break;
            }
            if ((cCode[cPC] >> 61) > 1)
                cPC++;
            cIndInst++;
        }

    }

};

#endif //CITYOFWEIRDFISHES_SCOMPANYCOMPILER_H
