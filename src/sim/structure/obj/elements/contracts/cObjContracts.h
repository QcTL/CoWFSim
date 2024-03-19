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

class con_b {
public:
    uint64_t ct_uuid;
    uint32_t ct_uuidCompanyGiving;
    uint32_t ct_strDate;

    con_b(uint32_t ctUuidCompanyGiving, uint32_t ctStrDate) : ct_strDate(ctStrDate),
                                                              ct_uuidCompanyGiving(ctUuidCompanyGiving) {}
};

class con_rentCell : public con_b {
public:
    uint32_t ct_uuidCompanyReceiving;

    uint32_t ct_endDate;
    con_TypePaymentFreq ct_typePayment;
    uint32_t ct_recurrentCost;
    uint32_t ct_totalCost;
    std::list<std::pair<int, int>> ct_lCells;

    con_rentCell(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate, uint32_t ctEndDate,
                 con_TypePaymentFreq ctTypePayment, uint32_t ctRecurrentCost, uint32_t ctTotalCost,
                 const std::list<std::pair<int, int>> &ctLCells) : con_b(ctUuidCompanyGiving, ctStrDate),
                                                                   ct_uuidCompanyReceiving(ctUuidCompanyReceiving),
                                                                   ct_endDate(ctEndDate),
                                                                   ct_typePayment(ctTypePayment),
                                                                   ct_recurrentCost(ctRecurrentCost),
                                                                   ct_totalCost(ctTotalCost),
                                                                   ct_lCells(ctLCells) {}
};


class con_buyCell : public con_b {
public:
    uint32_t ct_uuidCompanyReceiving;

    con_TypePaymentFreq ct_typePayment;
    uint32_t ct_recurrentCost;
    uint32_t ct_totalCost;
    std::list<std::pair<int, int>> ct_lCells;

    con_buyCell(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate,
                con_TypePaymentFreq ctTypePayment, uint32_t ctRecurrentCost, uint32_t ctTotalCost,
                const std::list<std::pair<int, int>> &ctLCells) : con_b(ctUuidCompanyGiving, ctStrDate),
                                                                  ct_uuidCompanyReceiving(ctUuidCompanyReceiving),
                                                                  ct_typePayment(ctTypePayment),
                                                                  ct_recurrentCost(ctRecurrentCost),
                                                                  ct_totalCost(ctTotalCost), ct_lCells(ctLCells) {}
};

class con_loanInteraction : public con_b {
public:
    uint32_t ct_uuidCompanyReceiving;

    con_TypePaymentFreq ct_typePayment;

    uint32_t ct_askedStart;
    uint32_t ct_recurrentCostReturn;
    uint32_t ct_totalCostReturn;

    con_loanInteraction(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate,
                        con_TypePaymentFreq ctTypePayment, uint32_t askedStart, uint32_t ctRecurrentCost,
                        uint32_t ctTotalCost)
            : con_b(ctUuidCompanyGiving, ctStrDate),
              ct_uuidCompanyReceiving(ctUuidCompanyReceiving),
              ct_typePayment(ctTypePayment),
              ct_recurrentCostReturn(ctRecurrentCost),
              ct_totalCostReturn(ctTotalCost),
              ct_askedStart(askedStart) {}
};

class con_stockInteraction : public con_b {
public:
    uint32_t ct_uuidCompanyReceiving;

    uint32_t ct_boughtStock;
    double ct_relativeTotalStock;

    con_stockInteraction(uint32_t ctUuidCompanyReceiving, uint32_t ctUuidCompanyGiving, uint32_t ctStrDate,
                         uint32_t boughtStock, double relativeTotalStock)
            : con_b(ctUuidCompanyGiving, ctStrDate),
              ct_uuidCompanyReceiving(ctUuidCompanyReceiving),
              ct_boughtStock(boughtStock),
              ct_relativeTotalStock(relativeTotalStock) {}
};


#endif //CITYOFWEIRDFISHES_COBJCONTRACTS_H
