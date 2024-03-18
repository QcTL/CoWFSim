//
// Created by Laminar on 17/03/2024.
//

#ifndef CITYOFWEIRDFISHES_COBJCONTRACTS_H
#define CITYOFWEIRDFISHES_COBJCONTRACTS_H

#include <cstdint>
#include <vector>

enum con_TypePaymentFreq {
    LC_PAY_MONTH, LC_PAY_YEAR, LC_PAY_FORTNIGHT
};

struct con_rentCell {
    uint64_t ct_uuid; //TODO
    uint32_t ct_uuidCompanyReceiving;
    uint32_t ct_uuidCompanyGiving;

    uint32_t ct_strDate;
    uint32_t ct_endDate;
    con_TypePaymentFreq ct_typePayment;
    uint32_t ct_recurrentCost;
    uint32_t ct_totalCost;
    std::list<std::pair<int, int>> ct_lCells;

    con_rentCell(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate, uint32_t ctEndDate,
                 con_TypePaymentFreq ctTypePayment, uint32_t ctRecurrentCost, uint32_t ctTotalCost,
                 const std::list<std::pair<int, int>> &ctLCells) : ct_uuidCompanyReceiving(
            ctUuidCompanyReceiving), ct_uuidCompanyGiving(ctUuidCompanyGiving), ct_strDate(ctStrDate),
                                                                   ct_endDate(ctEndDate),
                                                                   ct_typePayment(ctTypePayment),
                                                                   ct_recurrentCost(ctRecurrentCost),
                                                                   ct_totalCost(ctTotalCost),
                                                                   ct_lCells(ctLCells) {}
};


struct con_buyCell {
    uint64_t ct_uuid; //TODO
    uint32_t ct_uuidCompanyReceiving;
    uint32_t ct_uuidCompanyGiving;
    uint32_t ct_strDate;

    con_TypePaymentFreq ct_typePayment;
    uint32_t ct_recurrentCost;
    uint32_t ct_totalCost;
    std::list<std::pair<int, int>> ct_lCells;

    con_buyCell(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate,
                con_TypePaymentFreq ctTypePayment, uint32_t ctRecurrentCost, uint32_t ctTotalCost,
                const std::list<std::pair<int, int>> &ctLCells) : ct_uuidCompanyReceiving(ctUuidCompanyReceiving),
                                                                  ct_uuidCompanyGiving(ctUuidCompanyGiving),
                                                                  ct_strDate(ctStrDate), ct_typePayment(ctTypePayment),
                                                                  ct_recurrentCost(ctRecurrentCost),
                                                                  ct_totalCost(ctTotalCost), ct_lCells(ctLCells) {}
};

struct con_loanInteraction {
    uint64_t ct_uuid; //TODO
    uint32_t ct_uuidCompanyReceiving;
    uint32_t ct_uuidCompanyGiving;

    con_TypePaymentFreq ct_typePayment;

    uint32_t ct_askedStart;
    uint32_t ct_recurrentCostReturn;
    uint32_t ct_totalCostReturn;
};

struct con_stockInteraction {
    uint64_t ct_uuid; //TODO
    uint32_t ct_uuidCompanyReceiving;
    uint32_t ct_uuidCompanyGiving;

    uint32_t ct_boughtStock;
    double ct_relativeTotalStock;
};


#endif //CITYOFWEIRDFISHES_COBJCONTRACTS_H
