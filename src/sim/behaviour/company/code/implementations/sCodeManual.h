//
// Created by ganymede on 4/16/24.
//

#ifndef CITYOFWEIRDFISHES_SCODEMANUAL_H
#define CITYOFWEIRDFISHES_SCODEMANUAL_H

#include "isCode.h"
#include "../../../../groups/groupLand/sgTerrain.h"

class sCodeManual : public isCode {
public:

    sCodeManual() {
        if (snCommonAtr::getFlagAtr("snCA_Seed") != 0)
            sCS_genRVal.seed(snCommonAtr::getFlagAtr("snCA_Seed"));
        else
            sCS_genRVal.seed(static_cast<unsigned int>(time(nullptr)));
    }

    [[nodiscard]] std::shared_ptr<sCodeObj> getCodeByType(uint8_t inCompanySoilType) override {
        switch (inCompanySoilType) {
            case sgTerrain::sgT_TypeSoil::sgT_TS_T1Mixed:
                return getCivCodeAvailable();
            case sgTerrain::sgT_TypeSoil::sgT_TS_T2Mixed:
            case sgTerrain::sgT_TypeSoil::sgT_TS_T3Mixed:
                return getSmallCivCodeAvailable();
            case sgTerrain::sgT_TypeSoil::sgT_TS_T1Farm:
                return getFieldCodeAvailable();
            case sgTerrain::sgT_TypeSoil::sgT_TS_T1Industrial:
                return getIndCodeAvailable();
            case sgTerrain::sgT_TypeSoil::sgT_TS_T2Industrial:
                return getHIndCodeAvailable();
            default:
                return {};
        }
    }

private:

    std::mt19937 sCS_genRVal;
    std::vector<std::vector<uint64_t>> sCM_CodeSmallCiv{
            {
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000011,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0110000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100100000000000000000000000000000000000000000000000000000000011,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000010,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0110000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100100000000000000000000000000000000000000000000000000000000010,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },

    };
    [[nodiscard]] std::shared_ptr<sCodeObj> getSmallCivCodeAvailable() {
        std::uniform_int_distribution<> dis(0, (int) sCM_CodeSmallCiv.size() - 1);
        int randomIndex = dis(sCS_genRVal);
        return std::make_shared<sCodeObj>(sCM_CodeSmallCiv[randomIndex], 1000);
    }
    std::vector<std::vector<uint64_t>> sCM_CodesCiv{
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0110000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100100000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000110,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000110,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001111,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000110,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
    };

    [[nodiscard]] std::shared_ptr<sCodeObj> getCivCodeAvailable() {
        std::uniform_int_distribution<> dis(0, (int) sCM_CodesCiv.size() - 1);
        int randomIndex = dis(sCS_genRVal);
        return std::make_shared<sCodeObj>(sCM_CodesCiv[randomIndex], 1000);
    }

    std::vector<std::vector<uint64_t>> sCM_CodesField{
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100101000000000000000000000000000000000000000000000000000000110,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000110,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000110,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000001001110001000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100110000000000000000000000000000000000000000000000000000000110,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001111,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000110,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000001001110001000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001010,
                    (uint64_t) 0b1100110000000000000000000000000000000000000000000000000000000110,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001111,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
    };

    [[nodiscard]] std::shared_ptr<sCodeObj> getFieldCodeAvailable() {
        std::uniform_int_distribution<> dis(0, (int) sCM_CodesField.size() - 1);
        int randomIndex = dis(sCS_genRVal);
        return std::make_shared<sCodeObj>(sCM_CodesField[randomIndex], 1000);
    }

    std::vector<std::vector<uint64_t>> sCM_CodesInd = {
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000011,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000011,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001011,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000011,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001111,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000010,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000010,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001011,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000010,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001111,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000010,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000010,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001011,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000010,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000011,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000011,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001011,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000011,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
    };

    [[nodiscard]] std::shared_ptr<sCodeObj> getIndCodeAvailable() {
        std::uniform_int_distribution<> dis(0, (int) sCM_CodesInd.size() - 1);
        int randomIndex = dis(sCS_genRVal);
        return std::make_shared<sCodeObj>(sCM_CodesInd[randomIndex], 1000);
    }

    std::vector<std::vector<uint64_t>> sCM_CodesHInd = {
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001011,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b1000100000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000001100001101010000,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001111,
                    (uint64_t) 0b1100011000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
            {
                    (uint64_t) 0b1000001000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000000101,
                    (uint64_t) 0b1100000000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b1000101000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b1010000000000000000000000000000000000000000000000000000000000001,
                    (uint64_t) 0b0100000000000000000000000000000000000000000000000000000000000000,
                    (uint64_t) 0b0010000000000000000000000000000000000000000000000000000000001011,
                    (uint64_t) 0b1100010000000000000000000000000000000000000000000000000000000100,
                    (uint64_t) 0b1110000000000000000000000000000000000000000000000000000000000000,
            },
    };

    [[nodiscard]] std::shared_ptr<sCodeObj> getHIndCodeAvailable() {
        std::uniform_int_distribution<> dis(0, (int) sCM_CodesHInd.size() - 1);
        int randomIndex = dis(sCS_genRVal);
        return std::make_shared<sCodeObj>(sCM_CodesHInd[randomIndex], 1000);
    }
};

#endif //CITYOFWEIRDFISHES_SCODEMANUAL_H