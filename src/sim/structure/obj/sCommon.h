//
// Created by Laminar on 27/02/2024.
//

#ifndef CITYOFWEIRDFISHES_SCOMMON_H
#define CITYOFWEIRDFISHES_SCOMMON_H


/**
 * @struct retObjSLayerCells
 * @brief This struct is used as a return element for the function of setting the values of elements in each cell
 */
struct retObjSLayerCells {
    std::shared_ptr<gIGrid<uint8_t>> gUnderground;
    std::vector<std::vector<std::pair<int, int>>> routesMetro;
};

/**
 * @struct objActiveRute
 * @brief This struct defines a route that a car should be taking in its commute
 */
struct objActiveRute {
    std::pair<int, int> c_RStart;
    std::pair<int, int> c_REnd;
    bool c_IsReversed;
    uint64_t c_uuid;
};

/**
 * @struct sCodeObj
 * @brief This struct is used to save the code of a given company and its score
 */
struct sCodeObj {
    std::vector<uint64_t> sCO_Code;
    uint32_t sCO_Score = 0;

    sCodeObj(const std::vector<uint64_t> &sCoCode, uint32_t sCoScore) : sCO_Code(sCoCode), sCO_Score(sCoScore) {}
};

enum oPC_TypePayment {
    oPC_TP_RECURRENT_YEAR,
    oPC_TP_RECURRENT_MONTH,
    oPC_TP_RECURRENT_WEEK,
    oPC_TP_SOLD,
    oPC_TP_INVESTMENT_START,
    oPC_TP_CONTRACT_LOAN,
    oPC_TP_CONTRACT_ADQUIRE_CELL,
};

/**
 * @struct objPaymentCompany
 * @brief Thi struct is used to define a repeated payment given the different possible types of payment
 */
struct objPaymentCompany {
    double opc_qPayment;
    oPC_TypePayment opc_pType;
    uint32_t opc_pHour;
    uint32_t opc_pDate;
};


#endif //CITYOFWEIRDFISHES_SCOMMON_H
